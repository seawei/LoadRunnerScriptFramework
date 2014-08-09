// plugin.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

/*
    example protocol:
        <header><body>
 
        the length of body is stored in the header
*/


/**
 * creates your message
 * 
 * @author seawei (2014/8/9)
 * 
 * @param id        [IN] vuser id
 * @param msgid     [IN] message id
 * @param buf       [OUT] message buffer
 * @param len       [OUT] message length (Bytes)
 * 
 * @return int      returns 0 if no error occurs, or returns other value if error occurs
 */
PLUGIN_API int plugin_create_msg(const int id, 
                                 const int msgid, 
                                 char **buf, 
                                 int *len) {
    // You can use malloc to create your message by id and msgid.
    // (by your protocol's definition)
    // returns the message by setting buf and len.
    return 0;
}

/**
 * frees your message
 * 
 * @author seawei (2014/8/9)
 * 
 * @param buf   [IN] message buffer to free
 * 
 * @return void 
 */
PLUGIN_API void plugin_free_msg(char *buf) {
    // free your message by buf
    // free(buf);
}

/**
 * checks if the header of message is right (with your protocol's definition)
 * 
 * @author seawei (2014/8/9)
 * 
 * @param buf       [IN] message buffer (header only)
 * @param len       [IN] message length (header only) (Bytes)
 * @param id        [IN] vuser id
 * @param msgid     [IN] message id
 * @param msglen    [OUT] length of message body (got by the message header)
 * 
 * @return int      returns 0 if no error occurs, or returns other value if error occurs
 */
PLUGIN_API int plugin_check_header(const char *buf, 
                                   const int len, 
                                   const int id, 
                                   const int msgid, 
                                   int *msglen) {
    return 0; 
}

/**
 * checks if the body of message is right (with your protocol's definition)
 * 
 * @author seawei (2014/8/9)
 * 
 * @param buf       [IN] message buffer (body only)
 * @param len       [IN] message length (body only) (Bytes)
 * @param id        [IN] vuser id
 * @param msgid     [IN] message id
 * 
 * @return int      returns 0 if no error occurs, or returns other value if error occurs
 */
PLUGIN_API int plugin_check_body(const char *buf, 
                                 const int len, 
                                 const int id, 
                                 const int msgid) {
    return 0;
}