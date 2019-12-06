#ifndef SERVER_H
#define SERVER_H

extern "C" {

int logging_server(int sockfd);
int posting_server(int* sockfds, int this_id, int n_overall);
}

#endif /* SERVER_H */
