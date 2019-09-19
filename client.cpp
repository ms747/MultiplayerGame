#include <stdio.h>
#include <enet/enet.h>

int main()
{
    if (enet_initialize() != 0)
    {
        fprintf(stderr, "An error occurred while initializing ENet.\n");
        return EXIT_FAILURE;
    }

    ENetHost *client;
    ENetAddress ipaddress;
    ENetPeer *peer;

    client = enet_host_create(NULL, 1, 2, 0, 0);
    if (client == NULL)
    {
        fprintf(stderr,
                "An error occurred while trying to create an ENet server host.\n");
        exit(EXIT_FAILURE);
    }

    enet_address_set_host(&ipaddress, "localhost");
    ipaddress.port = 8080;

    peer = enet_host_connect(client, &ipaddress, 2, 0);

    if (peer == NULL)
    {
        printf("Unable to Connect to Server\n");
        return 0;
    }

    ENetEvent event;
    int connected = 0;
    if (enet_host_service(client, &event, 1000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT)
    {
        printf("Connection succeeded.\n");
        connected++;
    }
    else
    {
        enet_peer_reset(peer);
        printf("Could not connect\n");
        return 0;
    }

    while (1)
    {
        while (enet_host_service(client, &event, 1000) > 0)
        {
            switch (event.type)
            {
            case ENET_EVENT_TYPE_RECEIVE:
                puts((char *)event.packet->data);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                connected = 0;
                printf("You have been disconnected.\n");
                return 2;
            }
        }
    }




    enet_host_destroy(client);
    atexit(enet_deinitialize);
    return 0;
}