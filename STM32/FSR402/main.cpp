#include <mbed.h>
#include "FSR.h"
#include <stdio.h>
FSR fsr_left(A0, 2); // Pin 20 is used as the AnalogIn pin and a 10k resistor is used as a voltage divider
FSR fsr_right(A1, 2);
FSR fsr_back(A2, 0.2);
Serial pc(USBTX, USBRX);
WiFiInterface *wifi;
Thread thread;
volatile int l;
volatile int r;
volatile int b;
volatile float _es=-1,_e=-1,_ss=-1,_s=-1,_bs=-1,_lls=-1,_ls=-1,_rrs=-1,_rs=-1,_ll=-1,_l=-1,_rr=-1,_r=-1,_cs=-1,_c=-1,_bp=-1,now;
volatile float _ns=-1,_n=-1,_fs=-1,_f=-1,_bf=-1;
volatile int _sitst=0 , _lrst=2 ,_bfst=0;
volatile bool _empty=0,_tilt=0,_sit=0,_left=0,_right=0;
volatile bool l_s=0,l_e=0,l_ll=0,l_l=0,l_r=0,l_rr=0,l_c=0,l_n=0,l_f=0;

volatile int _tilt_status;
volatile bool running=true;
volatile float timesteady =1;
volatile float timeb =2;

const char *sec2str(nsapi_security_t sec)
{
    switch (sec) {
        case NSAPI_SECURITY_NONE:
            return "None";
        case NSAPI_SECURITY_WEP:
            return "WEP";
        case NSAPI_SECURITY_WPA:
            return "WPA";
        case NSAPI_SECURITY_WPA2:
            return "WPA2";
        case NSAPI_SECURITY_WPA_WPA2:
            return "WPA/WPA2";
        case NSAPI_SECURITY_UNKNOWN:
        default:
            return "Unknown";
    }
}

void tilt() {
    Timer _sit_t;
    _sit_t.start();
    while (running) {
        now = _sit_t.read();
        l=fsr_left.readWeight();
        r=fsr_right.readWeight();
        b=fsr_back.readWeight();

        if(l<=500&&r<=500){
            if(!l_e){//last time is sit
                _es=now;//empty start
                _s=-1;
                _ss=-1;//reset sit
                if(_bs==-1){_bs = now;}//if _bs not start=>start
                if(now-_bs>timeb&&_bs>0&&_sitst!=2){_sitst=0;}//tilt
            }
            else{
                if(_e==-1&&now-_es>timesteady){
                    _e = now;
                    _sitst=1;//empty
                    _bs =-1;
                }
                else if(now-_bs>timeb&&_bs>0&&_sitst!=2){_sitst=0;}//tilt
            }
            l_s=false;
            l_e=true;
        }
        else{
            if(!l_s){
                _ss=now;
                _e=-1;
                _es=-1;
                if(_bs==-1){_bs = now;}
                if(now-_bs>timeb&&_bs>0&&_sitst!=2){_sitst=0;}//tilt
            }
            else{
                if(_s==-1&&now-_ss>timesteady){
                    _s = now;
                    _sitst=3;//sit
                    _bs =-1;
                }
                else if(now-_bs>timeb&&_bs>0&&_sitst!=2){_sitst=0;}//tilt
                else if(now-_ss>=30&&_sitst!=4){_sitst=4;}//too long
            }
            l_s=true;
            l_e=false;
        }

        if(l>r*2.5&&l<=r*6){
            if(!l_l){
                _ls=now;
                _lls=-1,_rrs=-1,_rs=-1,_ll=-1,_l=-1,_rr=-1,_r=-1,_cs=-1,_c=-1,_cs=-1,_c=-1;
                if(_bp==-1){_bp = now;}
                if(now-_bp>timeb&&_bp!=-1){_lrst=5;}
            }
            else{
                if(_l==-1&&now-_ls>timesteady){
                    _l = now;
                    _lrst=1;//left
                    _bp =-1;
                }
                else if(now-_bp>timeb&&_bp!=-1){_lrst=5;}//tilt
            }
            l_r=false;l_rr=false;l_c=false;l_ll=false;
            l_l=true;
        }
        else if(l>r*6){
            if(!l_ll){
                _lls=now;
                _ls=-1,_rrs=-1,_rs=-1,_ll=-1,_l=-1,_rr=-1,_r=-1,_cs=-1,_c=-1;
                if(_bp==-1){_bp = now;}
                if(now-_bp>timeb&&_bp!=-1){_lrst=5;}
            }
            else{
                if(_ll==-1&&now-_lls>timesteady){
                    _ll = now;
                    _lrst=0;//very left
                    _bp =-1;
                }
                else if(now-_bp>timeb&&_bp!=-1){_lrst=5;}//tilt
            }
            l_r=false;l_rr=false;l_c=false;l_l=false;
            l_ll=true;
        }
        else if(l<=r*2.5&&r<=l*2.5){
            if(!l_c){
                _cs=now;
                _lls=-1,_ls=-1,_rrs=-1,_rs=-1,_ll=-1,_l=-1,_rr=-1,_r=-1,_c=-1;
                if(_bp==-1){_bp = now;}
                if(now-_bp>timeb&&_bp!=-1){_lrst=5;}
            }
            else{
                if(_c==-1&&now-_cs>timesteady){
                    _c = now;
                    _lrst=2;//very left
                    _bp =-1;
                }
                else if(now-_bp>timeb&&_bp!=-1){_lrst=5;}//tilt
            }
            l_r=false;l_rr=false;l_c=true;l_l=false;l_ll=false;
        }
        else if(r>l*2.5&&r<=l*6){
            if(!l_r){
                _lls=-1,_ls=-1,_rrs=-1,_rs=now,_ll=-1,_l=-1,_rr=-1,_r=-1,_c=-1,_cs=-1;;
                if(_bp==-1){_bp = now;}
                if(now-_bp>timeb&&_bp!=-1){_lrst=5;}
            }
            else{
                if(_r==-1&&now-_rs>timesteady){
                    _r = now;
                    _lrst=3;//very left
                    _bp =-1;
                }
                else if(now-_bp>timeb&&_bp!=-1){_lrst=5;}//tilt
            }
            l_r=true;l_rr=false;l_c=false;l_l=false;l_ll=false;
        }
        else if(r>l*6){
            if(!l_rr){
                _lls=-1,_ls=-1,_rrs=now,_rs=-1,_ll=-1,_l=-1,_rr=-1,_r=-1,_c=-1,_cs=-1;;
                if(_bp==-1){_bp = now;}
                if(now-_bp>timeb&&_bp!=-1){_lrst=5;}
            }
            else{
                if(_rr==-1&&now-_rrs>timesteady){
                    _rr = now;
                    _lrst=4;//very left
                    _bp =-1;
                }
                else if(now-_bp>timeb&&_bp!=-1){_lrst=5;}//tilt
            }
            l_r=false;l_rr=true;l_c=false;l_l=false;l_ll=false;
        }

        if(b<(l+r)/20){
            if(!l_f){
                _fs=now;
                _ns=-1;_n=-1;_f=-1;
                if(_bf==-1){_bf = now;}
                if(now-_bf>timeb&&_bf!=-1){_bfst=2;}
            }
            else{
                if(_f==-1&&now-_fs>timesteady){
                    _f = now;
                    _bfst=1;//left
                    _bf =-1;
                }
                else if(now-_bf>timeb&&_bf!=-1){_bfst=2;}//tilt
            }
            l_n=false;
            l_f=true;
        }
        else{
            if(!l_n){
                _ns=now;
                _fs=-1;_n=-1;_f=-1;
                if(_bf==-1){_bf = now;}
                if(now-_bf>timeb&&_bf!=-1){_bfst=2;}
            }
            else{
                if(_n==-1&&now-_ns>timesteady){
                    _n = now;
                    _bfst=0;//left
                    _bf =-1;
                }
                else if(now-_bf>timeb&&_bf!=-1){_bfst=2;}//tilt
            }
            l_n=true;
            l_f=false;
        }
+
        //printf("\rnow: %f ,(%5d,%5d,%5d)status: %d %d %d",now,l,r,b,_sitst,_lrst,_bfst);
        printf("\r%1d%1d%1d,%5d,%5d,%5d\0",_sitst,_lrst,_bfst,l,r,b);
        
        thread_sleep_for(100); //just here to slow down the output for easier reading
    }
}

int main(){

    printf("Hello~\r\n");
    printf("WiFi example\r\n");

    #ifdef MBED_MAJOR_VERSION
        printf("Mbed OS version %d.%d.%d\r\n\r\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
    #endif

    wifi = WiFiInterface::get_default_instance();
    if (!wifi) {
        printf("ERROR: No WiFiInterface found.\n");
        return -1;
    }
    
    printf("\nConnecting to %s...\n", MBED_CONF_APP_WIFI_SSID);
    int ret = wifi->connect(MBED_CONF_APP_WIFI_SSID, MBED_CONF_APP_WIFI_PASSWORD, NSAPI_SECURITY_WPA_WPA2);
    if (ret != 0) {
        printf("\nConnection error: %d\n", ret);
        return -1;
    }

    printf("Success\n\n");
    printf("MAC: %s\n", wifi->get_mac_address());
    printf("IP: %s\n", wifi->get_ip_address());
    printf("Netmask: %s\n", wifi->get_netmask());
    printf("Gateway: %s\n", wifi->get_gateway());
    printf("RSSI: %d\n\n", wifi->get_rssi());



    TCPSocket socket;
    int result = socket.open(wifi);

        
    if (result != 0) {
        printf("Error! socket.open() returned: %d\n", result);
    }

    result = socket.connect("192.168.43.8", 8700);
    //socket.set_blocking(0);
    if (result != 0) {
        printf("Error! socket.connect() returned: %d\n", result);
            // Close the socket to return its memory and bring down the network interface

    }
    int remaining = 256;
    int rcount = 0;
    char buffer[256];
    char sbuffer[] = "";
    //result = socket.send(sbuffer, sizeof(sbuffer));


    thread.start(tilt);
    // while(true){
    //     ThisThread::sleep_for(1);
    // }
    
    while(true){
        result = socket.recv(buffer, sizeof(buffer));
        
        if (result <= 0) {
            printf("Error! socket.recv() returned: %d\n\r", result);
            socket.close();
            wifi->disconnect();
            printf("break done due to error\n\r");
            break;
        }
        

        buffer[result]='\0';
        printf("Received %d bytes.\"%s\"\n\r", sizeof(buffer), buffer);
        char num_count[21];
        sprintf(num_count,"%1d%1d%1d,%5d,%5d,%5d",_sitst,_lrst,_bfst,l,r,b);
        //char * newArray = new char[0+15+1];
        //std::strcpy(newArray,sbuffer);
        //std::strcat(newArray,num_count);
        printf("send %s\n\r",num_count);
        //wait(1000);
        result = socket.send(num_count, sizeof(num_count));
        printf("Send %d bytes.\"%s\"\n\r", result, num_count);
        if (result <= 0) {
            printf("Error! socket.send() returned: %d\n\r", result);
            socket.close();
            wifi->disconnect();
            printf("break done due to error\n\r");
            break;
        }
    }
    
    socket.close();
    wifi->disconnect();
    running = false;
    thread.join();
    printf("ALL DONE!!\n\n");
    return 0;

}