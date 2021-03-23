#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "helpers.hpp"
#include "requests.hpp"

using namespace std;

int main(int argc, char *argv[]){

	char argument[100];
	char *host = (char*)malloc(10 * sizeof(char));
	strcpy(host, "3.8.116.10");
	char *reg = (char*)malloc(27 * sizeof(char));
	strcpy(reg, "/api/v1/tema/auth/register");
	char *content_type = (char*)malloc(34 * sizeof(char));
	strcpy(content_type, "application/json");
	char *login = (char*)malloc(24 * sizeof(char));
	strcpy(login, "/api/v1/tema/auth/login");
	char *response;
	char *message;
	char *cookie = (char*)malloc(100 * sizeof(char));
	char *token = (char*)malloc(300 * sizeof(char));
	char *access = (char*)malloc(27 * sizeof(char));
	strcpy(access, "/api/v1/tema/library/access");
	char *books = (char*)malloc(27 * sizeof(char));
	strcpy(books, "/api/v1/tema/library/books");
	char *logout = (char*)malloc(25 * sizeof(char));
	strcpy(logout, "/api/v1/tema/auth/logout");
	int sockfd;
	bool log = false;

	while(1){
		sockfd = open_connection(host, 8080, AF_INET, SOCK_STREAM, 0);

		cin >> argument;

		if (strcmp(argument, "exit") == 0){
			if(log == true){
				cout << "\n";
				cout << "Logging out...\n";
				char** cookies = (char**)calloc(1, sizeof(char*));

				cookies[0] = (char*)calloc(100, sizeof(char));
				strcpy(cookies[0], cookie);
				message = compute_get_request(host, logout, NULL, NULL, cookies, 1);

				send_to_server(sockfd, message);
				response = receive_from_server(sockfd);
				puts(response);
				free(response);
				free(message);

				free(cookies);
			}
			cout << "Exiting program...\n";
			break;
		}

		if (strcmp(argument, "register") == 0){
			char** user = (char**)calloc(2, sizeof(char*));
					cout << "username=";
					cin >> argument;

					user[0] = (char*)calloc(strlen(argument)+30, sizeof(char));
					strcpy(user[0],"\"username\": ");
					strcat(user[0], "\"");
					strcat(user[0], argument);
					strcat(user[0], "\"");
					strcat(user[0], "\0");

					cout << "password=";
					cin >> argument;
					user[1] = (char*)calloc(strlen(argument)+16, sizeof(char));
					strcpy(user[1],"\"password\": ");
					strcat(user[1], "\"");
					strcat(user[1], argument);
					strcat(user[1], "\"");
					strcat(user[1], "\0");

					message = compute_post_request(host, reg, content_type,
						 user, 2, NULL, NULL, 0);
					send_to_server(sockfd, message);
					response = receive_from_server(sockfd);

					cout << "\n";
					puts(response);
					free(response);
					free(message);
					free(user);

		}

		else if (strcmp(argument, "login") == 0){
			char** user = (char**)calloc(2, sizeof(char*));
			log = true;

					cout << "username=";
					cin >> argument;
					user[0] = (char*)calloc(strlen(argument)+16, sizeof(char));
					strcpy(user[0],"\"username\": ");
					strcat(user[0], "\"");
					strcat(user[0], argument);
					strcat(user[0], "\"");
					strcat(user[0], "\0");

					cout << "password=";
					cin >> argument;
					user[1] = (char*)calloc(strlen(argument)+16, sizeof(char));
					strcpy(user[1],"\"password\": ");
					strcat(user[1], "\"");
					strcat(user[1], argument);
					strcat(user[1], "\"");
					strcat(user[1], "\0");

					message = compute_post_request(host, login, content_type,
						user, 2, NULL, NULL, 0);
					send_to_server(sockfd, message);
				 	response = receive_from_server(sockfd);

					cout << "\n";
				 	puts(response);

					string dough(response); //cookie dough, din el facem cookie-ul:)
					size_t pos = dough.find("connect.sid=");
					dough = dough.substr(pos);
					pos = dough.find(";");
					dough = dough.substr(0, pos);
					strcpy(cookie, dough.c_str());

				 	free(response);
				 	free(message);
					free(user);
		}

		else if (strcmp(argument, "enter_library") == 0){
			char** cookies = (char**)calloc(1, sizeof(char*));

			cookies[0] = (char*)calloc(100, sizeof(char));
			strcpy(cookies[0], cookie);
			message = compute_get_request(host, access, NULL, NULL, cookies, 1);

			send_to_server(sockfd, message);
			response = receive_from_server(sockfd);
			puts(response);

			string code(response);
			size_t pos = code.find("{\"token\":\"");
			code = code.substr(pos);
			pos = code.find(":");
			code = code.substr(pos + 2);
			code = code.substr(0, code.length() - 2);
			strcpy(token, code.c_str());

			free(response);
			free(message);
			free(cookies);
		}


	else if (strcmp(argument, "get_books") == 0){

		message = compute_get_request(host, books, NULL, token, NULL, 0);

		send_to_server(sockfd, message);
		response = receive_from_server(sockfd);

		puts(response);

		free(response);
		free(message);
	}

	else if (strcmp(argument, "get_book") == 0){
		cout << "id=";
		cin >> argument;
		while (atoi(argument) <= 0){
			cout << "ID format not correct. Try again.\nid=";
			cin >> argument;
		}
		char *book = (char*)malloc(30 * sizeof(char));
		strcpy(book, books);
		strcat(book, "/");
		strcat(book, argument);
		message = compute_get_request(host, book, NULL, token, NULL, 0);

		send_to_server(sockfd, message);
		response = receive_from_server(sockfd);
		puts(response);

		free(response);
		free(message);
	}

	else if (strcmp(argument, "add_book") == 0){
		char** details = (char**)calloc(5, sizeof(char*));
				cout << "title=";
				cin >> argument;
				details[0] = (char*)calloc(strlen(argument) + 50, sizeof(char));
				strcpy(details[0],"\"title\": ");
				strcat(details[0], "\"");
				strcat(details[0], argument);
				strcat(details[0], "\"");
				strcat(details[0], "\0");

				cout << "author=";
				cin >> argument;
				details[1] = (char*)calloc(strlen(argument) + 50, sizeof(char));
				strcpy(details[1],"\"author\": ");
				strcat(details[1], "\"");
				strcat(details[1], argument);
				strcat(details[1], "\"");
				strcat(details[1], "\0");

				cout << "genre=";
				cin >> argument;
				details[2] = (char*)calloc(strlen(argument) + 50, sizeof(char));
				strcpy(details[2],"\"genre\": ");
				strcat(details[2], "\"");
				strcat(details[2], argument);
				strcat(details[2], "\"");
				strcat(details[2], "\0");

				cout << "publisher=";
				cin >> argument;
				details[3] = (char*)calloc(strlen(argument) + 50, sizeof(char));
				strcpy(details[3],"\"publisher\": ");
				strcat(details[3], "\"");
				strcat(details[3], argument);
				strcat(details[3], "\"");
				strcat(details[3], "\0");

				cout << "page_count=";
				cin >> argument;

				while (atoi(argument) <= 0){
					cout << "Page count format not correct. Try again.\npage_count=";
					cin >> argument;
				}

				details[4] = (char*)calloc(strlen(argument) + 30, sizeof(char));
				strcpy(details[4],"\"page_count\": ");
				strcat(details[4], "\"");
				strcat(details[4], argument);
				strcat(details[4], "\"");
				strcat(details[4], "\0");

				message = compute_post_request(host, books, content_type,
					details, 5, token, NULL, 0);
				send_to_server(sockfd, message);
				response = receive_from_server(sockfd);
				cout << "\n";
				puts(response);

				free(response);
				free(message);
				free(details);
	}

	else if (strcmp(argument, "delete_book") == 0){
		char *book = (char*)malloc(30 * sizeof(char));

		cout << "id=";
		cin >> argument;

		while (atoi(argument) <= 0){
			cout << "ID format not correct. Try again.\nid=";
			cin >> argument;
		}

		strcpy(book, books);
		strcat(book, "/");
		strcat(book, argument);

		message = compute_delete_request(host, book, token,
			content_type, content_type);
		send_to_server(sockfd, message);
		response = receive_from_server(sockfd);
		puts(response);

		free(book);
		free(response);
	}

	else if(strcmp(argument, "logout") == 0){
		char** cookies = (char**)calloc(1, sizeof(char*));
		log = false;

		cookies[0] = (char*)calloc(100, sizeof(char));
		strcpy(cookies[0], cookie);
		message = compute_get_request(host, logout, NULL, NULL, cookies, 1);

		send_to_server(sockfd, message);
		response = receive_from_server(sockfd);
		puts(response);
		free(response);
		free(message);

		free(cookies);
	}

	else {
		cout << "Invalid command. These are the available commands: \n";
		cout << "	register: register a new user\n";
		cout << "	login: login with a registered user\n";
		cout << "	enter_library: request library access\n";
		cout << "	get_books: request all the available books from server\n";
		cout << "	get_book: request book information based on its ID\n";
		cout << "	add_book: add a new book\n";
		cout << "	delete_book: delete a book based on its ID\n";
		cout << "	logout: logout the current user\n";
		cout << "	exit: quit the program\n";
	}

	close(sockfd);

}

	free(host);
	free(reg);
	free(content_type);
	free(cookie);
	free(access);
	free(token);
	free(books);
	free(logout);

	return 0;
}
