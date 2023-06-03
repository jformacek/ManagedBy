//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//	1 - 10 ... Error events
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//
//  Values are 32 bit values laid out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//


//
// Define the severity codes
//
#define STATUS_SEVERITY_SUCCESS          0x0
#define STATUS_SEVERITY_INFORMATIONAL    0x1
#define STATUS_SEVERITY_WARNING          0x2
#define STATUS_SEVERITY_ERROR            0x3


//
// MessageId: S_GET_COMPUTER
//
// MessageText:
//
// Could not get computer object from AD. Error %1.
//
#define S_GET_COMPUTER                   0xC0000001L

//
// MessageId: S_NOT_ADDED
//
// MessageText:
//
// Could not add member to local Administrators group.%n
// Member:%t%1%n
// Error:%t%2%n
//
#define S_NOT_ADDED                      0xC0000002L

//
// MessageId: S_NOT_TRANSLATED
//
// MessageText:
//
// Could not translate AD object to security principal.%n
// Object:%t%1%n
// Error:%t%2%n
//
#define S_NOT_TRANSLATED                 0xC0000003L

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//	11 - 20 ... Success events
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//
// MessageId: S_NO_ADMIN
//
// MessageText:
//
// List of managers is empty. Nothing to do.
//
#define S_NO_ADMIN                       0x0000000BL

//
// MessageId: S_ADDED
//
// MessageText:
//
// Object %1 added to local Administrators group. 
//
#define S_ADDED                          0x0000000CL

//
// MessageId: S_ALREADY_MEMBER
//
// MessageText:
//
// Object %1 is already member of local Administrators group. Nothing to do.
//
#define S_ALREADY_MEMBER                 0x0000000DL

//
// MessageId: S_FINISHED
//
// MessageText:
//
// Finished successfully.
//
#define S_FINISHED                       0x0000000EL

