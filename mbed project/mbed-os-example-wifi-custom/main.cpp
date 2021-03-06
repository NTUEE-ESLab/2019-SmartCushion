/* WiFi Example
 * Copyright (c) 2016 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mbed.h"
#include <iostream>
using namespace std;
WiFiInterface *wifi;

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

int scan_demo(WiFiInterface *wifi)
{
    WiFiAccessPoint *ap;

    printf("Scan:\n");

    int count = wifi->scan(NULL,0);

    if (count <= 0) {
        printf("scan() failed with return value: %d\n", count);
        return 0;
    }

    /* Limit number of network arbitrary to 15 */
    count = count < 15 ? count : 15;

    ap = new WiFiAccessPoint[count];
    count = wifi->scan(ap, count);

    if (count <= 0) {
        printf("scan() failed with return value: %d\n", count);
        return 0;
    }

    for (int i = 0; i < count; i++) {
        printf("Network: %s secured: %s BSSID: %hhX:%hhX:%hhX:%hhx:%hhx:%hhx RSSI: %hhd Ch: %hhd\n", ap[i].get_ssid(),
               sec2str(ap[i].get_security()), ap[i].get_bssid()[0], ap[i].get_bssid()[1], ap[i].get_bssid()[2],
               ap[i].get_bssid()[3], ap[i].get_bssid()[4], ap[i].get_bssid()[5], ap[i].get_rssi(), ap[i].get_channel());
    }
    printf("%d networks available.\n", count);

    delete[] ap;
    return count;
}

int main()
{
    printf("WiFi example\n");

#ifdef MBED_MAJOR_VERSION
    printf("Mbed OS version %d.%d.%d\n\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
#endif

    wifi = WiFiInterface::get_default_instance();
    if (!wifi) {
        printf("ERROR: No WiFiInterface found.\n");
        return -1;
    }
/* 
    int count = scan_demo(wifi);
    if (count == 0) {
        printf("No WIFI APs found - can't continue further.\n");
        return -1;
    }
*/
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
    int result;
    result = socket.open(wifi);
    // Send a simple http request
    
/*
    char sbuffer[] = "hello:)";
    nsapi_size_t size = strlen(sbuffer);
*/

    if (result != 0) {
        printf("Error! socket.open() returned: %d\n", result);
    }

    result = socket.connect("192.168.43.8", 8700);
    if (result != 0) {
        printf("Error! socket.connect() returned: %d\n", result);
            // Close the socket to return its memory and bring down the network interface

    }

    // Loop until whole request sent

//    result = socket.send(sbuffer, size);

    int pos;
    int posarray[10]={1,1,1,1,2,2,2,3,0};
    int count=0;
    //0 for no one sitting on the chair
    while(pos)
    {
        //pos = get_position_result();
        pos = posarray[count];
        //1 means the user sits straight
        if(pos == 1)
        {
            char sbuffer[] = "Good Posture";
            nsapi_size_t size = strlen(sbuffer);
        }
        //2 means bad posture
        else if(pos == 2)
        {
            char sbuffer[] = "BAD Posture";
            nsapi_size_t size = strlen(sbuffer);
            pos = socket.send(sbuffer, size);
        }
        //3 means sitting too long
        else if(pos == 3)
        {
            char sbuffer[] = "Sitting TOO long";
            nsapi_size_t size = strlen(sbuffer);
            pos = socket.send(sbuffer, size);
        }
        else{
            char sbuffer[] = "leave";
            nsapi_size_t size = strlen(sbuffer);
            pos = socket.send(sbuffer, size);   
        }
        count++;
    }

    // Receieve an HTTP response and print out the response line
    int remaining = 256;
    int rcount = 0;
    char p[256];
    result = socket.recv(p, remaining);

    if (result < 0) {
        printf("Error! socket.recv() returned: %d\n", result);
            // Close the socket to return its memory and bring down the network interface
        socket.close();

        // Bring down the ethernet interface
        wifi->disconnect();
        printf("\nDone\n");
    }
	// the HTTP response code
    //printf("recv %d [%.*s]\n", rcount, strstr(buffer, "\r\n")-buffer, buffer);

    cout << p;

    //delete[] buffer;


    // Close the socket to return its memory and bring down the network interface
    socket.close();

    // Bring down the ethernet interface
    wifi->disconnect();
    printf("\nDone\n");
}
