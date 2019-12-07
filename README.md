# SPTH
A crawler, crawling through WikiPedia in order to find paths to Hitler!
---

Starting from a random page (or a specified page given via ARGV) and crawling all the way to the page of Adolf Hitler.

## Features

#### Currently supports:
* Multithreaded search
* Caching, in order to prevent re-requesting the same page
* Reading configs from file, rather than hard-coding

#### Options to add:
* Re-creating threads upon end (was too lazy)
* Zipping the cached files
* Making it a bit more asynchronous

## Dependencies & Installation:
##### Native installation:
* Make & CMake: On Arch linux, install via `pacman -S make cmake`, On Ubuntu, install via `apt install make cmake`
* cURLpp: On Arch linux, install from [AUR](https://aur.archlinux.org/packages/libcurlpp/), On Ubuntu, install via `apt install libcurlpp0 libcurl4-openssl-dev libcurlpp-dev`

* Install the repo itself:
  * get the code, using this command: `git clone https://github.com/lidordublin/spth.git spth` 
  * cd into the repository directory: `cd spth`
  * Then, build and compile: `cmake . && make`

##### Run inside Docker:
* Install the repo itself, as specified in the [Native installation](https://github.com/LidorDublin/SPTH#native-installation) section
* Docker: On Arch linux, install via `pacman -S docker`, On Ubuntu, install via `apt install docker`
* build the docker image: `docker build -t spth .`

Then run!

## Usage

`./shortest-path-to-hitler` - Runs with a random page as a starting point

`./shortest-path-to-hitler PAGE` - Runs with PAGE as a starting point

To run in Docker: `docker run --rm -v $(pwd)/.cache:/spth/.cache it spth:latest`

---

# Credits
Thanks to [Uriya Hapeness](https://github.com/uriyaharpeness) for giving me the idea for this fun project

Thanks to [JanivBaby](https://github.com/yaniv0409) for helping me around sometimes and doing some CRs and of-course - laughing at me when needed 

Special thanks to [Niels lohmann](https://github.com/nlohmann/json) for his amazing open-source JSON library, and to [cURLpp project](https://github.com/jpbarrette/curlpp) for the amazing open-source URL-transfer library that were a core part of the integration with WikiAPI.

Last-but-not-least: Thanks to [Israel Railways](https://www.rail.co.il/en) for being slow enough for me to be able to code when commuting


###### Note: Any CRs and options to add are welcome!
