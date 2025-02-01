#include <arpa/inet.h>
#include <limits.h>
#include <netdb.h>
#include <openssl/sha.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// #include "logger.h"

#define IP "152.53.18.76"
#define PORT 8080

char *itoa(int num, char *str) {
  /* Int to string */
  if (str == NULL)
    return NULL;
  sprintf(str, "%d", num);
  return str;
}

int main(int argc, char *argv[]) {
  setvbuf(stdout, NULL, _IONBF, 0);

#if defined(__arm__)
#ifdef COLORS
  printf("\e[1;34mPlatform: ARM\e[0m\n");
  printf("\e[1;33mSpec: Hashrate measure unsupported because platform "
         "scepifics\e[0m\n");
#else
  printf("Platform: ARM\n");
  printf("Spec: Hash measure unsupported because platform scepifics\n");
#endif
#elif defined(__aarch64__)
#ifdef COLORS
  printf("\e[1;34mPlatform: ARM64\e[0m\n");
#else
  printf("Platform: ARM64\n");
#endif
#else
  print_info("Compiled for other\n")
#endif

  printf("\n");

  char *hostname = malloc(HOST_NAME_MAX);
  if (gethostname(hostname, sizeof(hostname)) != 0) {
#ifdef COLORS
    printf("\e[1;33m[warning]\e[0m get hostname!\n");
    printf("\e[1;33m[warning]\e[0m setting hostname to worker1\n");
#else
    printf("[warning] get hostname!\n");
    printf("[warning] setting hostname to worker1\n");
#endif
    hostname = "worker1";
  }

  int sockfd;
  struct sockaddr_in server_addr;

  // Create Suckit
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
#ifdef COLORS
    printf("\e[1;31m[error]\e[0m when creating socket!");
#else
    printf("[error] when creating socket!");
#endif
    exit(EXIT_FAILURE);
  }

  // Server Setup
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = inet_addr(IP);

  // Connect
  if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
      0) {
#ifdef COLORS
    printf("\e[1;31m[error]\e[0m connect to duco server!");
#else
    printf("[error] connect to duco server!");
#endif
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  // Get Server Version
  char *server_version = malloc(8);
  if (recv(sockfd, server_version, 3, 0) < 0) {
#ifdef COLORS
    printf("\e[1;31m[error]\e[0m receive version!");
#else
    printf("[error] recv version!");
#endif
    return 1;
  } else {
#ifdef COLORS
    printf("\e[1;35m[server]\e[0m version: %s\n", server_version);
#else
    printf("[server] version: %s\n", server_version);
#endif
  }
  free((void *)server_version);

  // Send SYN
  char *motd = malloc(128);
  send(sockfd, "MOTD", strlen("MOTD"), 0);
  recv(sockfd, motd, sizeof(motd), 0);
  free((void *)motd);

  // Server Say
  char *server_say = malloc(256);
  // memset(server_say, 0, 256);
  if (recv(sockfd, server_say, 256, 0) < 0) {
#ifdef COLORS
    printf("\e[1;31m[error]\e[0m recv job!");
#else
    printf("[error] recv job!");
#endif
    return 1;
  }
#ifdef COLORS
  printf("\e[1;35m[server]\e[0m %s\n", server_say);
#else
  printf("[server] %s\n", server_say);
#endif
  free((void *)server_say);

  if (argc < 3) {
#ifdef COLORS
    printf("\e[1;34m[info]\e[0m  %s username mining_key\n", argv[0]);
#else
    printf("[info]use: %s username mining_key\n", argv[0]);
#endif
    return 1;
  }
  char *job = malloc(64);
  sprintf(job, "%s,%s,%s,%s", "JOB", argv[1], "LOW", argv[2]);

  const int replay_size = 256;
  char *serverreply = malloc(replay_size);
  clock_t start, end;
  double diff_t;
  int accepted_shares, rejected_shares;
  accepted_shares = rejected_shares = 0;

  while (1) {
    memset(serverreply, 0, replay_size);
    // printf("job.msg: %s\n", job);
    if (send(sockfd, job, strlen(job), 0) < 0) {
      printf("Couldn't send JOB message\n");
      return 1;
    }
    if (recv(sockfd, serverreply, replay_size, 0) < 0) {
      printf("Couldn't receive job\n");
      return 1;
    }
    // printf("serv.rep: %s\n", serverreply);
    /* Split received data */
    char *job = strtok(serverreply, ",");
    char *work = strtok(NULL, ",");
    char *diff = strtok(NULL, "");
    // printf("Recived data:\n");
    // printf("JOB:  %s\n", job);
    // printf("JOB2: %s\n", work);
    // printf("DIFF: %s\n", diff);
    printf("\e[1;36m[net]\e[0m \e[1;35mreceived new job ...\e[0m diff: \e[1;37m%s\e[0m", diff);
    char *ducos1_result_string = malloc(16);
    start = clock(); // Начало замера времени
    // memset(ducos1_result_string, 0x0, 16);
    for (int i = 0; i < (100 * atoi(diff)) + 1; i++) {
      char str_to_hash[128] = "";

      strcat(str_to_hash, job);

      itoa(i, ducos1_result_string);
      strcat(str_to_hash, ducos1_result_string);

      // printf("String to hash: %s\n", str_to_hash);

      unsigned char temp[SHA_DIGEST_LENGTH];
      char buf[SHA_DIGEST_LENGTH * 2];

      memset(buf, 0x0, SHA_DIGEST_LENGTH * 2);
      memset(temp, 0x0, SHA_DIGEST_LENGTH);

      SHA1((unsigned char *)str_to_hash, strlen(str_to_hash), temp);

      long iZ = 0;
      for (iZ = 0; iZ < SHA_DIGEST_LENGTH; iZ++)
        sprintf((char *)&(buf[iZ * 2]), "%02x", temp[iZ]);

      // printf("Hashed res    : %s\n", buf);
      // printf("Expected res  : %s\n\n", work);

      if (strcmp(work, buf) == 0) {
        // printf("Found share! %s\n", ducos1_result_string);

        /* Calculate hashrate */
        end = clock();
        diff_t =
            ((double)(end - start)) / CLOCKS_PER_SEC; // Переводим в секунды
        int hashrate = i / diff_t;
        // printf("Hashrate: %d\n", hashrate);
        // printf("%d:%f\n", i, diff_t);
        if (hashrate <= 0 || (int)diff_t == 0)
          hashrate = 1000;

        char *result = malloc(256);
        sprintf(result, "%s,%d,%s", ducos1_result_string, hashrate, hostname);
        // printf("Result: %s\n", result);

        if (send(sockfd, result, strlen(result), 0) < 0) { // send result
          printf("Can't send result!\n");
          return 1;
        }
        free(result);

        char feedback[20] = "";
        if (recv(sockfd, feedback, 20, 0) < 0) { // receive feedback
          printf("Error: Feedback couldn't be received\n");
          return 1;
        }

        // printf("Feedback: %s\n", feedback);
        if (strcmp("GOOD\n", feedback) == 0 ||
            strcmp("BLOCK\n", feedback) == 0) {
          accepted_shares++;
          printf("\e[1;32m[cpu] accepted \e[0m %d/%d share: %s hashrate: "
                 "\e[1;35m%d H/s\e[0m "
                 "feedback: \e[1;32m%s\e[0m",
                 accepted_shares, rejected_shares, ducos1_result_string,
                 hashrate, feedback);

        } else if (strcmp("INVU\n", feedback) == 0) {
          printf("Error: Incorrect username\n");
          return 1;

        } else {
          rejected_shares++;
          printf("\e[1;31m[cpu] rejected \e[0m %d/%d share: %s hashrate: "
                 "\e[1;35m%d H/s\e[0m "
                 "feedback: \e[1;31m%s\e[0m",
                 accepted_shares, rejected_shares, ducos1_result_string,
                 hashrate, feedback);
        }
        free(ducos1_result_string);
        break;
      }
    }
  }

  printf("Success\n");
  close(sockfd);
  free((void *)hostname);
  return 0;
}
