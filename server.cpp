#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <enet/enet.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

int main()
{
    int result = SDL_Init(SDL_INIT_EVERYTHING);
    bool running = true;
    SDL_Window *window;
    SDL_Renderer *render;
    if (result < 0)
    {
        printf("Failed to Load SDL\n");
    }
    else
    {
        window = SDL_CreateWindow("Server", 100, 100, 500, 500, 4);
        if (window == nullptr)
        {
            printf("Failed to Load Graphics Drivers\n");
        }
        else
        {
            render = SDL_CreateRenderer(window, -1, 2);
        }
    }

    int id = 0;
    if (enet_initialize() != 0)
    {
        fprintf(stderr, "An error occurred while initializing ENet.\n");
        return EXIT_FAILURE;
    }

    ENetAddress address;
    ENetHost *server;

    address.host = ENET_HOST_ANY;
    address.port = 8080;

    server = enet_host_create(&address, 2, 2, 0, 0);
    if (server == NULL)
    {
        fprintf(stderr,
                "An error occurred while trying to create an ENet server host.\n");
        exit(EXIT_FAILURE);
    }

    ENetEvent event;
    ENetPacket *packet;
    char buffer[20];
    char sdl_buffer[50];
    memset(sdl_buffer, 0, 50);
    SDL_Event sdlevent;

    while (running)
    {
        while (enet_host_service(server, &event, 10) > 0 || SDL_PollEvent(&sdlevent))
        {
            switch (sdlevent.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            default:
                break;
            }

            switch (event.type)
            {
            case ENET_EVENT_TYPE_CONNECT:
                printf("Client Connected\n");
                sprintf(buffer, "Online players : %d", ++id);
                packet = enet_packet_create(buffer, strlen(buffer) + 1, 0);
                enet_host_broadcast(server, 1, packet);
                break;
            case ENET_EVENT_TYPE_RECEIVE:
                printf("Client connected\n");
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                printf("Client disconnected\n");
                --id;
                break;
            default:
                break;
            }
        }
        SDL_SetRenderDrawColor(render, 0, 0, 0, 0xFF);
        SDL_RenderClear(render);
        sprintf(sdl_buffer, "Connected Clients : %d", id);
        stringColor(render, 250, 250, sdl_buffer, 0xFF0000FF);
        SDL_RenderPresent(render);
        memset(sdl_buffer,0,50);
        SDL_Delay(1);
    }

    enet_host_destroy(server);
    atexit(enet_deinitialize);
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}