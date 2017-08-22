#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <SDL.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <linux/if.h>
#include <netdb.h>
#include <unistd.h>

int main(int argc, char** argv)
{
	SDL_Color colors[256];
	int color_ind = 0;

	struct ifaddrs *addrs,*tmp;

	getifaddrs(&addrs);
	tmp = addrs;

	while (tmp)
	{
	    if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_PACKET)
	    {
		struct ifreq s;
		int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

		strcpy(s.ifr_name, tmp->ifa_name);
		if (0 == ioctl(fd, SIOCGIFHWADDR, &s)) 
		{
			printf("%s\n", s.ifr_name);
			colors[color_ind].r = s.ifr_addr.sa_data[0];
			colors[color_ind].g = s.ifr_addr.sa_data[1];
			colors[color_ind].b = s.ifr_addr.sa_data[2];
			++color_ind;
			colors[color_ind].r = s.ifr_addr.sa_data[3];
			colors[color_ind].g = s.ifr_addr.sa_data[4];
			colors[color_ind].b = s.ifr_addr.sa_data[5];
			++color_ind;
		}

		close(fd);
            }

	    tmp = tmp->ifa_next;
	}

	freeifaddrs(addrs);	

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window * window = SDL_CreateWindow("Color MAC", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 640, 0);
	SDL_Renderer * ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	
	SDL_Event evt;
	while (true)
	{
		if (SDL_PollEvent(&evt))
		{
			if (evt.type == SDL_QUIT) break;
		}

		SDL_SetRenderDrawColor(ren, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(ren);

		SDL_Rect rect;
		rect.x = 0;
		rect.y = 0;
		rect.w = 128;
		rect.h = 128;
		for (int i = 0; i < color_ind; ++i) 
		{
			SDL_SetRenderDrawColor(ren, colors[i].r, colors[i].g, colors[i].b, 0xFF);
			SDL_RenderFillRect(ren, &rect);
			rect.x += rect.w;
			if (rect.x >= 640) 
			{
				rect.x = 0;
				rect.y += rect.h;
			}
		}

		SDL_RenderPresent(ren);
	}

	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}
