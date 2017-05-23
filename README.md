NetCraft
========

Voxel game for modern web browsers (Chrome, Firefox, Safari) and desktop operating systems (Windows, macOS, Linux).
Just a few thousand lines of C using modern OpenGL (shaders). Online multiplayer support is included using a Python-based server
or through [WebSandboxMC](https://github.com/satoshinm/WebSandboxMC/).

**Live demo: https://satoshinm.github.io/NetCraft/**

[![CircleCI](https://circleci.com/gh/satoshinm/NetCraft.svg?style=svg)](https://circleci.com/gh/satoshinm/NetCraft)

[![AppVeyor](https://ci.appveyor.com/api/projects/status/asql58oxt14pfva4?svg=true)](https://ci.appveyor.com/project/satoshinm/netcraft)

Based on Michael Fogleman's [Craft](https://github.com/fogleman/Craft):

http://www.michaelfogleman.com/craft/

Supports running in a web browser with WebGL (built using emscripten), including multiplayer through WebSockets:

![Screenshot](http://i.imgur.com/Fw2wQL7.png)

and also natively, as with the original Craft:

![Screenshot](http://i.imgur.com/SH7wcas.png)

### Features

* Simple but nice looking terrain generation using perlin / simplex noise.
* More than 10 types of blocks and more can be added easily.
* Supports plants (grass, flowers, trees, etc.) and transparency (glass).
* Simple clouds in the sky (they don't move).
* Day / night cycles and a textured sky dome.
* World changes persisted in a sqlite3 database.
* Multiplayer support!

### Download

JavaScript, WebAssembly, and Linux builds are available from CircleCI: https://circleci.com/gh/satoshinm/NetCraft

Windows builds are available from AppVeyor: https://ci.appveyor.com/project/satoshinm/netcraft

See below to run from source.

### Web builds

To build for the web, compiling to JavaScript, first install [Emscripten](http://emscripten.org).
The EM SDK is the easiest to install, but to get my patch fixes you can either build from source
using this branch: https://github.com/satoshinm/emscripten/commits/netcraft, or alternatively
install 1.37.12 from the SDK and apply a patch:

    patch -p1 -d $EMSCRIPTEN < src/emscripten-1.37.12+netcraftfixes.patch

Once your Emscripten environment is setup, then run:

    git clone https://github.com/satoshinm/NetCraft.git
    cd NetCraft
    mkdir build
    cmake -DCMAKE_TOOLCHAIN_FILE=$EMSCRIPTEN/cmake/Modules/Platform/Emscripten.cmake ..
    make

then open craft.html in your HTML5/WebGL-compatible web browser, and it will load craft.js. The game should
function similarly to the native build (see below).

Web-specific code is marked in the C source with `#ifdef __EMSCRIPTEN__`.


#### Optimized web builds

The default emscripten build configuration is unoptimized, for ease of development, but you
can optimize using `-DCMAKE_BUILD_TYPE=Release` to cmake, for example:

    mkdir release-build-js
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=$EMSCRIPTEN/cmake/Modules/Platform/Emscripten.cmake ..
    make

This process takes longer, but generates a minified craft.js (using `-O3` to emcc), several
times smaller. The optimized builds may require running from a web server instead
of a local file due to origin restrictions, an easy way to do this is using Python:

    python -m SimpleHTTPServer

then browsing to http://localhost:8000/craft.html.

#### WebAssembly web build

Finally to enable the use of [WebAssembly](http://webassembly.org) (through
emscripten's `-s WASM` flag to emcc), pass `-DWASM=1` to cmake:

    mkdir wasm-build
    cmake -DWASM=1 -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=$EMSCRIPTEN/cmake/Modules/Platform/Emscripten.cmake ..
    make

This creates a binary file `craft.wasm` with the bulk of the application code,
instead of JavaScript, for better performance. Note not all browsers support WebAssembly yet (check [caniuse](https://caniuse.com/#feat=wasm))
and this build type should be considered very experimental.

### Native builds

Building natively for desktop operating systems is also supported.

You can have both native and web builds in different directories, building from the same
source. This is useful for testing web-based regressions and ensuring native functionality.
I use the following directories, each you can run `make` from for different build types:

* build
* release-build-js
* wasm-build
* native-build (see below)

#### macOS

Download and install [CMake](http://www.cmake.org/cmake/resources/software.html)
if you don't already have it. You may use [Homebrew](http://brew.sh) to simplify
the installation:

    brew install cmake

#### Linux (Ubuntu 16.04.2 LTS)

    sudo apt install cmake libglew-dev xorg-dev libcurl4-openssl-dev
    sudo apt install libglfw3-dev

#### Windows

Download and install [CMake](http://www.cmake.org/cmake/resources/software.html)
and [MinGW](http://www.mingw.org/). Add `C:\MinGW\bin` to your `PATH`.

Download and install [cURL](http://curl.haxx.se/download.html) so that
CURL/lib and CURL/include are in your Program Files directory.

Use the following commands in place of the ones described in the next section.

    cmake -G "MinGW Makefiles"
    mingw32-make

### Compile and Run

Once you have the dependencies (see above), run the following commands in your
terminal.

    mkdir native-build
    cd native-build
    cmake ..
    make
    ./craft

To enable additional compiler debug settings, run cmake with:

    cmake -DDEBUG=1 ..

#### Client

You can connect to a server with command line arguments...

    ./craft craft.michaelfogleman.com

Or, with the "/online" command in the game itself.

    /online craft.michaelfogleman.com

In the web-based version, you can pass command-line arguments after `#` in the URL.


#### Server

You can run your own server or connect to mine. The server is written in Python
but requires a compiled DLL so it can perform the terrain generation just like
the client.

    gcc -std=c99 -O3 -fPIC -shared -o world -I src -I deps/noise deps/noise/noise.c src/world.c
    python server.py [HOST [PORT]]

Multiplayer is supported in both the native and web clients. To connect to a server
from the web client, the server must support WebSockets; the Python server can be bridged
using [websockify](https://github.com/novnc/websockify) as follows:

    ./websockify.py 4081 localhost:4080

Or for an alternative Java-based Bukkit plugin WebSocket server, check out:

    https://github.com/satoshinm/WebSandboxMC/

The web-based client will connect to WebSockets at the path `/craftws` by default. Full
URLs can be given in place of the hostname to connect at other paths, or to use SSL (wss://).
`/online localhost` is equivalent to `/online ws://localhost:4081/craftws`. The special
hostname "-" can be used to connect to the same server the page is served from.

### Controls

Mouse:

- Left mouse button (hold) to destroy a block.
- Right Click or Ctrl + Left Click to create a block.
- Ctrl + Right Click to toggle a block as a light source.
- Middle Click to change the current block selection to the targeted block.
- Scrollwheel to cycle through the block types, or zoom in orthogonal mode.
- Drag-and-drop files onto the canvas to change [textures](#textures).

Keyboard:

- WASD to move forward, left, backward, right.
- Space to jump.
- Space double-tap to toggle between walking and flying (when flying, space to ascend, shift to descend).
- Shift to crouch, walk slower.
- Tab to sprint, while holding another directional key (increases movement speed).
- 1-9 to select the block type to create.
- E or R to cycle through the block types.
- ZXCVBN to move in exact directions along the XYZ axes.
- Z to zoom.
- F to show the scene in orthographic mode.
- When in orthographic mode, use the scrollwheel or '+', '-' keys to zoom in and out.
- O to observe players in the main view.
- P to observe players in the picture-in-picture view.
- T or Enter to type text into chat (use up/down arrow keys to scroll through history).
- Forward slash (/) to enter a command.
- Backquote (`) to write text on any block (signs).
- Arrow keys emulate mouse movement.
- `[` emulates left mouse click.
- `]` emulates right mouse click.
- F11 to toggle fullscreen/windowed mode.
- F4 to toggle VR side-by-side stereo rendering.
- Shift-F4 to toggle barrel distortion effect.
- F2 to take a screenshot on the next scene render.
- F3 to toggle debug info text.
- F1 to toggle all user interface elements.

Joystick:

- A or D-pad up to jump.
- Left stick to move.
- Right stick to look.
- L1 and R1 bumpers to cycle through the block types.
- L2 trigger to destroy a block.
- R2 trigger to create a block.
- D-pad left/right to move left/right.
- D-pad down to crouch.

Touch:

- Swipe to free look.
- 2-finger touch to walk forward.
- 3-finger tap to jump.
- Tap to destroy a block.
- Tap left edge to create a block.

### Chat Commands

    /goto [NAME]

Teleport to another user.
If NAME is unspecified, a random user is chosen.

    /list

Display a list of connected users.

    /login NAME

Switch to another registered username.
The login server will be re-contacted. The username is case-sensitive.

    /logout

Unauthenticate and become a guest user.
Automatic logins will not occur again until the /login command is re-issued.

    /offline [FILE]

Switch to offline mode.
FILE specifies the save file to use and defaults to "craft".

    /online HOST [PORT]

Connect to the specified server.

    /pq P Q

Teleport to the specified chunk.

    /spawn

Teleport back to the spawn point.

### Screenshot

![Screenshot](http://i.imgur.com/foYz3aN.png)

### Implementation Details

#### Terrain Generation

The terrain is generated using Simplex noise - a deterministic noise function seeded based on position. So the world will always be generated the same way in a given location.

The world is split up into 32x32 block chunks in the XZ plane (Y is up). This allows the world to be “infinite” (floating point precision is currently a problem at large X or Z values) and also makes it easier to manage the data. Only visible chunks need to be queried from the database.

#### Rendering

Only exposed faces are rendered. This is an important optimization as the vast majority of blocks are either completely hidden or are only exposing one or two faces. Each chunk records a one-block width overlap for each neighboring chunk so it knows which blocks along its perimeter are exposed.

Only visible chunks are rendered. A naive frustum-culling approach is used to test if a chunk is in the camera’s view. If it is not, it is not rendered. This results in a pretty decent performance improvement as well.

Chunk buffers are completely regenerated when a block is changed in that chunk, instead of trying to update the VBO.

Text is rendered using a bitmap atlas. Each character is rendered onto two triangles forming a 2D rectangle.

“Modern” OpenGL is used - no deprecated, fixed-function pipeline functions are used. Vertex buffer objects are used for position, normal and texture coordinates. Vertex and fragment shaders are used for rendering. Matrix manipulation functions are in matrix.c for translation, rotation, perspective, orthographic, etc. matrices. The 3D models are made up of very simple primitives - mostly cubes and rectangles. These models are generated in code in cube.c.

Transparency in glass blocks and plants (plants don’t take up the full rectangular shape of their triangle primitives) is implemented by discarding magenta-colored pixels in the fragment shader.

#### Textures

Textures are based on:

* [Craft](https://github.com/fogleman/Craft) by Michael Fogleman, MIT
* Pixeludi Pack by Wojtek Mroczek, Creative Commons CC-BY-SA, 2011, as used in [TrueCraft](https://github.com/SirCmpwn/TrueCraft/).
* [Piehole Plain](http://piehole.alexvoelk.de) by Alex Voelk, CC-BY-SA 3.0

Custom textures can be loaded from the server, or by dragging-and-dropping a .zip file or .png texture atlas
onto the game. NetCraft uses its own texture format, although there is some interoperability provided.
To customize textures you can start with [/textures/texture.png](/textures/texture.png), or (may not work
completely as expected), seek out an old texture pack for Minecraft 1.4.7 or earlier, such as
[Pixeludi](https://github.com/SirCmpwn/TrueCraft/blob/7f0e3338d231b8a8f7fba35742b618a4fb3575bf/TrueCraft.Client/Content/terrain.png),
[Piehole](http://piehole.alexvoelk.de), or
[Sphax PureBDcraft 16x](http://bdcraft.net/purebdcraft-minecraft#older).
Only texture atlas images are supported, in a manner somewhat compatible with [pre-1.5](http://minecraft.gamepedia.com/Texture_pack#Converting_texture_packs_to_resource_packs)
packs. High-resolution texture packs and packs for 1.5+ or newer, including resource packs, are not supported!


#### Database

User changes to the world are stored in a sqlite database. Only the delta is stored, so the default world is generated and then the user changes are applied on top when loading.

The main database table is named “block” and has columns p, q, x, y, z, w. (p, q) identifies the chunk, (x, y, z) identifies the block position and (w) identifies the block type. 0 represents an empty block (air).

In game, the chunks store their blocks in a hash map. An (x, y, z) key maps to a (w) value.

The y-position of blocks are limited to 0 <= y < 256. The upper limit is mainly an artificial limitation to prevent users from building unnecessarily tall structures. Users are not allowed to destroy blocks at y = 0 to avoid falling underneath the world.

#### Multiplayer

Multiplayer mode is implemented using plain-old sockets. A simple, ASCII, line-based protocol is used. Each line is made up of a command code and zero or more comma-separated arguments. The client requests chunks from the server with a simple command: C,p,q,key. “C” means “Chunk” and (p, q) identifies the chunk. The key is used for caching - the server will only send block updates that have been performed since the client last asked for that chunk. Block updates (in realtime or as part of a chunk request) are sent to the client in the format: B,p,q,x,y,z,w. After sending all of the blocks for a requested chunk, the server will send an updated cache key in the format: K,p,q,key. The client will store this key and use it the next time it needs to ask for that chunk. Player positions are sent in the format: P,pid,x,y,z,rx,ry. The pid is the player ID and the rx and ry values indicate the player’s rotation in two different axes. The client interpolates player positions from the past two position updates for smoother animation. The client sends its position to the server at most every 0.1 seconds (less if not moving).

Client-side caching to the sqlite database can be performance intensive when connecting to a server for the first time. For this reason, sqlite writes are performed on a background thread. All writes occur in a transaction for performance. The transaction is committed every 5 seconds as opposed to some logical amount of work completed. A ring / circular buffer is used as a queue for what data is to be written to the database.

In multiplayer mode, players can observe one another in the main view or in a picture-in-picture view. Implementation of the PnP was surprisingly simple - just change the viewport and render the scene again from the other player’s point of view.

#### Collision Testing

Hit testing (what block the user is pointing at) is implemented by scanning a ray from the player’s position outward, following their sight vector. This is not a precise method, so the step rate can be made smaller to be more accurate.

Collision testing simply adjusts the player’s position to remain a certain distance away from any adjacent blocks that are obstacles. (Clouds and plants are not marked as obstacles, so you pass right through them.)

#### Sky Dome

A textured sky dome is used for the sky. The X-coordinate of the texture represents time of day. The Y-values map from the bottom of the sky sphere to the top of the sky sphere. The player is always in the center of the sphere. The fragment shaders for the blocks also sample the sky texture to determine the appropriate fog color to blend with based on the block’s position relative to the backing sky.

#### Ambient Occlusion

Ambient occlusion is implemented as described on this page:

http://0fps.wordpress.com/2013/07/03/ambient-occlusion-for-minecraft-like-worlds/

#### Dependencies

* GLEW is used for managing OpenGL extensions across platforms.
* GLFW is used for cross-platform window management.
* CURL is used for HTTPS / SSL POST for the authentication process.
* lodepng is used for loading PNG textures.
* miniz is used for writing PNG screenshots and loading zip archives.
* sqlite3 is used for saving the blocks added / removed by the user.
* tinycthread is used for cross-platform threading.
* emscripten is used for compiling to the web platform.
