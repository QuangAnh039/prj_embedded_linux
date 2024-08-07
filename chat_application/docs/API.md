void *handle_connection(void *ptr);
void *input_handler(void *ptr);
void list_connections();
void terminate_connection(int id);
void send_message(int id, const char *message);
void show_myip();
void show_myport();
void connect_to_peers(const char *ip, int ports);
