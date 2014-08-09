/**
 * utility functions and macros to send and receive messages
 * with plugin
 * 
 * written by seawei, 20140809
 */

#ifndef __LR_SEAWEI_SOCKET_H__
#define __LR_SEAWEI_SOCKET_H__

// size of temporary buffer (may need be changed by your protocol)
#define SEAWEI_BUF_SIZE			100

// length of message header (may need be changed by your protocol)
#define SEAWEI_BYTES_HEADER 	8

// timeout for connecting to server (seconds)
#define SEAWEI_TIMEOUT_CONNECT	3

/*
	sends a request and receive the response,
	with detailed transactions (including '<tran>', and two sub-transactions - '<tran>_req' and '<tran>_ack')

parameters:
	uid 		[IN] vuser id
	tran 		[IN] transaction name (send requeset + receive reponse)
	socket  	[IN] socket name
	msgid_req	[IN] message id for request
	msgid_ack   [IN] message id for response
*/
int send_recv_with_detailed_tran(const int uid, 
								 char *tran, 
								 char *socket, 
								 const int msgid_req, 
								 const int msgid_ack)
{
    char sub_tran_req[SEAWEI_BUF_SIZE];
    char sub_tran_ack[SEAWEI_BUF_SIZE];
    	
    char * buf_send;
	int buf_send_len=0;
	char * buf_recv;
	int buf_recv_len;
	int msg_len;
    
    char szTmp[SEAWEI_BUF_SIZE];
	int ret;

	// create the names of sub-transactions
    sprintf(sub_tran_req, "%s_req", tran);
    sprintf(sub_tran_ack, "%s_ack", tran);

	// start main transaction
	lr_start_transaction(tran);

	// start the sub-transaction of sending the request
	lr_start_sub_transaction(sub_tran_req, tran);
	{
		// create a message by calling the plugin function
		ret = plugin_create_msg(uid, msgid_req, &buf_send, &buf_send_len);
		if (ret) {
			lr_end_sub_transaction(sub_tran_req, LR_FAIL);
			lr_end_transaction(tran, LR_FAIL);
			return 1;
		}

		// sends the request
		lrs_set_send_buffer(socket, buf_send, buf_send_len);
		lrs_length_send(socket, "buf_send", NoOption, LrsLastArg);

		// frees the request buffer after sending
		plugin_free_msg(buf_send);
	}
	lr_end_sub_transaction(sub_tran_req, LR_AUTO);

	// start the sub-transaction of receving the response
	lr_start_sub_transaction(sub_tran_ack, tran);
	{
		// receives the header of the response
		sprintf(szTmp, "NumberOfBytesToRecv=%d", SEAWEI_BYTES_HEADER);
		lrs_receive_ex(socket, "buf_recv", szTmp, LrsLastArg);
		lrs_get_last_received_buffer(socket, &buf_recv, &buf_recv_len);

		// checks the header of the response by calling the plugin function
		ret = plugin_check_header(buf_recv, buf_recv_len, uid, msgid_ack, &msg_len);
		if (ret) {
			lrs_free_buffer(buf_recv);			
			lr_end_sub_transaction(sub_tran_ack, LR_FAIL);
			lr_end_transaction(tran, LR_FAIL);
			return 2;
		}
		lrs_free_buffer(buf_recv);

		// receives the body of the response
		sprintf(szTmp, "NumberOfBytesToRecv=%d", msg_len);
		lrs_receive_ex(socket, "buf_recv", szTmp, LrsLastArg);
		lrs_get_last_received_buffer(socket, &buf_recv, &buf_recv_len);

		// checks the body of the response by calling the plugin function
		ret = plugin_check_body(buf_recv, buf_recv_len, uid, msgid_ack);
		if (ret) {
			lrs_free_buffer(buf_recv);
			lr_end_sub_transaction(sub_tran_ack, LR_FAIL);
			lr_end_transaction(tran, LR_FAIL);
			return 3;
		}
		lrs_free_buffer(buf_recv);
	}
	lr_end_sub_transaction(sub_tran_ack, LR_AUTO);
	lr_end_transaction(tran, LR_AUTO);
    
    return 0;
}

/*
	sends a request and receive the response,
	with simple transactions (including '<tran>' only)

parameters:
	uid 		[IN] vuser id
	tran 		[IN] transaction name (send requeset + receive reponse)
	socket  	[IN] socket name
	msgid_req	[IN] message id for request
	msgid_ack   [IN] message id for response
*/
int send_recv_with_simple_tran(const int uid, 
							   char *tran, 
							   char *socket, 
							   const int msgid_req, 
							   const int msgid_ack)
{
    char sub_tran_req[SEAWEI_BUF_SIZE];
    char sub_tran_ack[SEAWEI_BUF_SIZE];
    	
    char * buf_send;
	int buf_send_len=0;
	char * buf_recv;
	int buf_recv_len;
	int msg_len;
    
    char szTmp[SEAWEI_BUF_SIZE];
	int ret;

    // start transaction
	lr_start_transaction(tran);
	{
        // create a message by calling the plugin function
		ret = plugin_create_msg(uid, msgid_req, &buf_send, &buf_send_len);
		if (ret) {
			lr_end_transaction(tran, LR_FAIL);
			return 1;
		}

        // sends the request
		lrs_set_send_buffer(socket, buf_send, buf_send_len);
		lrs_length_send(socket, "buf_send", NoOption, LrsLastArg);

        // frees the request buffer after sending
		plugin_free_msg(buf_send);
	}

	{
		// receives the header of the response
		sprintf(szTmp, "NumberOfBytesToRecv=%d", SEAWEI_BYTES_HEADER);
		lrs_receive_ex(socket, "buf_recv", szTmp, LrsLastArg);
		lrs_get_last_received_buffer(socket, &buf_recv, &buf_recv_len);

        // checks the header of the response by calling the plugin function
		ret = plugin_check_header(buf_recv, buf_recv_len, uid, msgid_ack, &msg_len);
		if (ret) {
			lrs_free_buffer(buf_recv);			
			lr_end_transaction(tran, LR_FAIL);
			return 2;
		}
		lrs_free_buffer(buf_recv);

		// receives the body of the response
		sprintf(szTmp, "NumberOfBytesToRecv=%d", msg_len);
		lrs_receive_ex(socket, "buf_recv", szTmp, LrsLastArg);
		lrs_get_last_received_buffer(socket, &buf_recv, &buf_recv_len);

        // checks the body of the response by calling the plugin function
		ret = plugin_check_body(buf_recv, buf_recv_len, uid, msgid_ack);
		if (ret) {
			lrs_free_buffer(buf_recv);
			lr_end_transaction(tran, LR_FAIL);
			return 3;
		}
		lrs_free_buffer(buf_recv);
	}
	lr_end_transaction(tran, LR_AUTO);
    
    return 0;
}

/*
	sends a request and receive the response without any transactions	

parameters:
	uid 		[IN] vuser id
	socket  	[IN] socket name
	msgid_req	[IN] message id for request
	msgid_ack   [IN] message id for response
*/
int send_recv_without_tran(const int uid, 
						   char *socket, 
						   const int msgid_req, 
						   const int msgid_ack)
{
    char sub_tran_req[SEAWEI_BUF_SIZE];
    char sub_tran_ack[SEAWEI_BUF_SIZE];
    	
    char * buf_send;
	int buf_send_len=0;
	char * buf_recv;
	int buf_recv_len;
	int msg_len;
    
    char szTmp[SEAWEI_BUF_SIZE];
	int ret;
    
	{
        // create a message by calling the plugin function
		ret = plugin_create_msg(uid, msgid_req, &buf_send, &buf_send_len);
		if (ret) {
			return 1;
		}

        // sends the request
		lrs_set_send_buffer(socket, buf_send, buf_send_len);
		lrs_length_send(socket, "buf_send", NoOption, LrsLastArg);

        // frees the request buffer after sending
		plugin_free_msg(buf_send);
	}

	{
		// receives the header of the response
		sprintf(szTmp, "NumberOfBytesToRecv=%d", SEAWEI_BYTES_HEADER);
		lrs_receive_ex(socket, "buf_recv", szTmp, LrsLastArg);
		lrs_get_last_received_buffer(socket, &buf_recv, &buf_recv_len);

        // checks the header of the response by calling the plugin function
		ret = plugin_check_header(buf_recv, buf_recv_len, uid, msgid_ack, &msg_len);
		if (ret) {
			lrs_free_buffer(buf_recv);			
			return 2;
		}
		lrs_free_buffer(buf_recv);

		// receives the body of the response
		sprintf(szTmp, "NumberOfBytesToRecv=%d", msg_len);
		lrs_receive_ex(socket, "buf_recv", szTmp, LrsLastArg);
		lrs_get_last_received_buffer(socket, &buf_recv, &buf_recv_len);

        // checks the body of the response by calling the plugin function
		ret = plugin_check_body(buf_recv, buf_recv_len, uid, msgid_ack);
		if (ret) {
			lrs_free_buffer(buf_recv);
			return 3;
		}
		lrs_free_buffer(buf_recv);
	}
    
    return 0;
}

/*
	returns ret with printing a error message if ret is not 0

parameters:
	ret 		[IN] a returned value
	name 		[IN] a description string
*/
#define ASSERT_F(ret, name) \
{\
	if (ret) { \
		lr_error_message(name##" failed: %d", ret);\
		return ret;\
	}\
}

/*
	tests with a rendezvous point with detailed transactions (see send_recv_with_detailed_tran)

parameters:
	id 			[IN] vuser id
	msgid		[IN] message id
	server  	[IN] server socket name
	tran 		[IN] transaction name (send requeset + receive reponse)
*/
#define LR_TEST1(id, msgid_req, msgid_ack, server, tran) \
{\
	lr_rendezvous("r_"##tran);\
	ret = send_recv_with_detailed_tran(id, tran, server, msgid | FLAG_REQ, msgid | FLAG_ACK);\
	ASSERT_F(ret, tran);\
}

/*
	tests with a rendezvous point with simple transactions (see send_recv_with_simple_tran)

parameters:
	id 			[IN] vuser id
	msgid		[IN] message id
	server  	[IN] server socket name
	tran 		[IN] transaction name (send requeset + receive reponse)
*/
#define LR_TEST(id, msgid, server, tran) \
{\
	lr_rendezvous("r_"##tran);\
	ret = send_recv_with_simple_tran(id, tran, server, msgid | FLAG_REQ, msgid | FLAG_ACK);\
	ASSERT_F(ret, tran);\
}

/*
	tests with a rendezvous point without any transactions (see send_recv_without_tran)

parameters:
	id 			[IN] vuser id
	msgid		[IN] message id
	server  	[IN] server socket name
	name 		[IN] a description string
*/
#define LR_TEST0(id, msgid, server, name) \
{\
	ret = send_recv_without_tran(id, server, msgid | FLAG_REQ, msgid | FLAG_ACK);\
	ASSERT_F(ret, name);\
}

#endif
