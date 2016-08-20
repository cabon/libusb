/*
    $ sudo apt-get install libusb-1.0-0-dev
    $ gcc -o libusb libusb.c `pkg-config --libs --cflags libusb-1.0`

   */

#include <stdio.h>
#include <stdlib.h>
#include <libusb.h>

void printdev(libusb_device *dev);
void printConf(struct libusb_config_descriptor *conf);
void printInterface(const struct libusb_interface *interface);
void printEndpoint(const struct libusb_endpoint_descriptor *endpoint);

int main(){
    libusb_device **devs;
    libusb_context *ctx = NULL;
    int r, i;
    ssize_t cnt;

    r = libusb_init(&ctx);
    if(r < 0){
        fprintf(stderr, "[-] Init\n");
        return 1;
    }

    libusb_set_debug(ctx, 3);
    cnt = libusb_get_device_list(ctx, &devs);
    if(cnt < 0){
        fprintf(stderr, "[-] Get devices\n");
        return 1;
    }

    printf("[+] Devs list: %d\n", (int)cnt);
    for(i=0; i<cnt; i++){
        printdev(devs[i]);
    }

    libusb_free_device_list(devs, 1);
    libusb_exit(ctx);
    return 0;
}

void printdev(libusb_device *dev){
    struct libusb_device_descriptor desc;
    struct libusb_config_descriptor *conf;

    uint8_t busNum;
    uint8_t devNum;
    int r, r2;
    char vendor[BUFSIZ], product[BUFSIZ];
    
    busNum = libusb_get_bus_number(dev);
    devNum = libusb_get_device_address(dev);

    r = libusb_get_device_descriptor(dev, &desc);

    if(r < 0){
        fprintf(stderr, "Get device desc\n");
        return;
    }
    
    r2 = libusb_get_config_descriptor(dev, 0, &conf);

    if(r2 < 0){
        fprintf(stderr, "Get device conf\n");
        return;
    }

    printf("Bus %03u Device %03u: ID 0x%04x:0x%04x\n", \
            busNum, devNum,\
            desc.idVendor, desc.idProduct);
    printf("  bLength: %d\n", desc.bLength);
    printf("  bDescriptorType: %d\n", desc.bDescriptorType);
    printf("  bcdUSB: %u\n", desc.bcdUSB);
    printf("  bDeviceClass: %u\n", desc.bDeviceClass);
    printf("  bDeviceSubClass: %u\n", desc.bDeviceSubClass);
    printf("  bDeviceProtocol: %u\n", desc.bDeviceProtocol);
    printf("  bMaxPacketSize: %u\n", desc.bMaxPacketSize0);
    printf("  idVendor: 0x%04x\n", desc.idVendor);
    printf("  idProduct: 0x%04x\n", desc.idProduct);
    printf("  bcdDevice: %u\n", desc.bcdDevice);
    printf("  iManufacturer: %d\n", desc.iManufacturer);
    printf("  iProduct: %u\n", desc.iProduct);
    printf("  iSerialNumber: %u\n", desc.iSerialNumber);
    printf("  bNumConfigurations: %u\n", desc.bNumConfigurations);
    printConf(conf);
}
 
void printConf(struct libusb_config_descriptor *conf){
    int i;

    printf("  Configuration Descriptor\n");
    printf("\tbLength\t\t\t%d\n", conf->bLength);
    printf("\tbDescriptorType\t\t%d\n", conf->bDescriptorType);
    printf("\twTotalLength\t\t%d\n", conf->wTotalLength);
    printf("\tbNumInterfaces\t\t%d\n", conf->bNumInterfaces);
    printf("\tbConfigurationValue\t%d\n", conf->bConfigurationValue);
    printf("\tiConfiguration\t\t%d\n", conf->iConfiguration);
    printf("\tbmAttributes\t\t0x%02X\n", conf->bmAttributes);
    printf("\tMaxPower\t\t%dmA\n", conf->MaxPower);

    for(i=0; i<conf->bNumInterfaces; i++)
        printInterface(&conf->interface[i]);
}


void printInterface(const struct libusb_interface *interface){
    const struct libusb_interface_descriptor *ifd;
    int i, j;

    printf("\tInterface Descriptor\n");
    for(i=0; i < (interface->num_altsetting); i++){
        ifd = &interface->altsetting[i];
        printf("\t\tbLength: %d\n", ifd->bLength);
        printf("\t\tbDescriptorType: %d\n", ifd->bDescriptorType);
        printf("\t\tbInterfaceNumber: %d\n", ifd->bInterfaceNumber);
        printf("\t\tbAlternateSetting: %d\n", ifd->bAlternateSetting);
        printf("\t\tbNumEndpoints: %d\n", ifd->bNumEndpoints);
        printf("\t\tbInterfaceClass: %d\n", ifd->bInterfaceClass);
        printf("\t\tbInterfaceSubClass: %d\n", ifd->bInterfaceSubClass);
        printf("\t\tbInterfaceProtocol: %d\n", ifd->bInterfaceProtocol);
        printf("\t\tiInterface: %d\n", ifd->iInterface);

        for(j=0; j<ifd->bNumEndpoints; j++)
            printEndpoint(&ifd->endpoint[i]);
    }
}

void printEndpoint(const struct libusb_endpoint_descriptor *endpoint){
    
    printf("\t\t\tEndpoint Descriptor\n");
    printf("\t\t\tbLength: %d\n", endpoint->bLength);
    printf("\t\t\tbDescriptorType: %d\n", endpoint->bDescriptorType);
    printf("\t\t\tbEndpointAddress: 0x%02X\n", endpoint->bEndpointAddress);
    printf("\t\t\tbmAttributes: %d\n", endpoint->bmAttributes);
    printf("\t\t\twMaxPacketSize: 0x%04X\n", endpoint->wMaxPacketSize);
    printf("\t\t\tbInterval: %d\n", endpoint->bInterval);
}
