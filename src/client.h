#ifndef _client_h_
#define _client_h_

#ifdef __EMSCRIPTEN__
#define DEFAULT_PORT 4081
#else
#define DEFAULT_PORT 4080
#endif

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

void client_enable(void);
void client_disable(void);
bool get_client_enabled(void);
void client_connect(char *hostname, int port);
void client_start(void);
void client_stop(void);
void client_send(char *data);
void client_message(int fd, void *userData);
char *client_recv(void);
void client_version(int version);
void client_login(const char *username, const char *identity_token);
void client_position(float x, float y, float z, float rx, float ry);
void client_chunk(int p, int q, int key);
void client_block(int x, int y, int z, int w);
void client_light(int x, int y, int z, int w);
void client_sign(int x, int y, int z, int face, const char *text);
void client_talk(const char *text);

#endif
