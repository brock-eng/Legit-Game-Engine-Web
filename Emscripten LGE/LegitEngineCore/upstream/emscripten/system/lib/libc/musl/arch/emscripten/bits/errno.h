#include <wasi/api.h>

#define EPERM              __WASI_ERRNO_PERM
#define ENOENT             __WASI_ERRNO_NOENT
#define ESRCH              __WASI_ERRNO_SRCH
#define EINTR              __WASI_ERRNO_INTR
#define EIO                __WASI_ERRNO_IO
#define ENXIO              __WASI_ERRNO_NXIO
#define E2BIG              __WASI_ERRNO_2BIG
#define ENOEXEC            __WASI_ERRNO_NOEXEC
#define EBADF              __WASI_ERRNO_BADF
#define ECHILD             __WASI_ERRNO_CHILD
#define EAGAIN             __WASI_ERRNO_AGAIN
#define ENOMEM             __WASI_ERRNO_NOMEM
#define EACCES             __WASI_ERRNO_ACCES
#define EFAULT             __WASI_ERRNO_FAULT
#define EBUSY              __WASI_ERRNO_BUSY
#define EEXIST             __WASI_ERRNO_EXIST
#define EXDEV              __WASI_ERRNO_XDEV
#define ENODEV             __WASI_ERRNO_NODEV
#define ENOTDIR            __WASI_ERRNO_NOTDIR
#define EISDIR             __WASI_ERRNO_ISDIR
#define EINVAL             __WASI_ERRNO_INVAL
#define ENFILE             __WASI_ERRNO_NFILE
#define EMFILE             __WASI_ERRNO_MFILE
#define ENOTTY             __WASI_ERRNO_NOTTY
#define ETXTBSY            __WASI_ERRNO_TXTBSY
#define EFBIG              __WASI_ERRNO_FBIG
#define ENOSPC             __WASI_ERRNO_NOSPC
#define ESPIPE             __WASI_ERRNO_SPIPE
#define EROFS              __WASI_ERRNO_ROFS
#define EMLINK             __WASI_ERRNO_MLINK
#define EPIPE              __WASI_ERRNO_PIPE
#define EDOM               __WASI_ERRNO_DOM
#define ERANGE             __WASI_ERRNO_RANGE
#define EDEADLK            __WASI_ERRNO_DEADLK
#define ENAMETOOLONG       __WASI_ERRNO_NAMETOOLONG
#define ENOLCK             __WASI_ERRNO_NOLCK
#define ENOSYS             __WASI_ERRNO_NOSYS
#define ENOTEMPTY          __WASI_ERRNO_NOTEMPTY
#define ELOOP              __WASI_ERRNO_LOOP
#define ENOMSG             __WASI_ERRNO_NOMSG
#define EIDRM              __WASI_ERRNO_IDRM
#define ENOLINK            __WASI_ERRNO_NOLINK
#define EPROTO             __WASI_ERRNO_PROTO
#define EMULTIHOP          __WASI_ERRNO_MULTIHOP
#define EBADMSG            __WASI_ERRNO_BADMSG
#define EOVERFLOW          __WASI_ERRNO_OVERFLOW
#define EILSEQ             __WASI_ERRNO_ILSEQ
#define ENOTSOCK           __WASI_ERRNO_NOTSOCK
#define EDESTADDRREQ       __WASI_ERRNO_DESTADDRREQ
#define EMSGSIZE           __WASI_ERRNO_MSGSIZE
#define EPROTOTYPE         __WASI_ERRNO_PROTOTYPE
#define ENOPROTOOPT        __WASI_ERRNO_NOPROTOOPT
#define EPROTONOSUPPORT    __WASI_ERRNO_PROTONOSUPPORT
#define EAFNOSUPPORT       __WASI_ERRNO_AFNOSUPPORT
#define EADDRINUSE         __WASI_ERRNO_ADDRINUSE
#define EADDRNOTAVAIL      __WASI_ERRNO_ADDRNOTAVAIL
#define ENETDOWN           __WASI_ERRNO_NETDOWN
#define ENETUNREACH        __WASI_ERRNO_NETUNREACH
#define ENETRESET          __WASI_ERRNO_NETRESET
#define ECONNABORTED       __WASI_ERRNO_CONNABORTED
#define ECONNRESET         __WASI_ERRNO_CONNRESET
#define ENOBUFS            __WASI_ERRNO_NOBUFS
#define EISCONN            __WASI_ERRNO_ISCONN
#define ENOTCONN           __WASI_ERRNO_NOTCONN
#define ETIMEDOUT          __WASI_ERRNO_TIMEDOUT
#define ECONNREFUSED       __WASI_ERRNO_CONNREFUSED
#define EHOSTUNREACH       __WASI_ERRNO_HOSTUNREACH
#define EALREADY           __WASI_ERRNO_ALREADY
#define EINPROGRESS        __WASI_ERRNO_INPROGRESS
#define ESTALE             __WASI_ERRNO_STALE
#define EDQUOT             __WASI_ERRNO_DQUOT
#define ECANCELED          __WASI_ERRNO_CANCELED
#define EOWNERDEAD         __WASI_ERRNO_OWNERDEAD
#define ENOTRECOVERABLE    __WASI_ERRNO_NOTRECOVERABLE

// Codes without a wasi equivalent, make sure they start
// above the wasi ones, which are dense [1,76].
// Also try to fit the codes in a single byte signed wasm SLEB.

#define ENOSTR          100
#define EBFONT          101
#define EBADSLT         102
#define EBADRQC         103
#define ENOANO          104
#define ENOTBLK         105
#define ECHRNG          106
#define EL3HLT          107
#define EL3RST          108
#define ELNRNG          109
#define EUNATCH         110
#define ENOCSI          111
#define EL2HLT          112
#define EBADE           113
#define EBADR           114
#define EXFULL          115
#define ENODATA         116
#define ETIME           117
#define ENOSR           118
#define ENONET          119
#define ENOPKG          120
#define EREMOTE         121
#define EADV            122
#define ESRMNT          123
#define ECOMM           124
#define EDOTDOT         125
#define ENOTUNIQ        126
#define EBADFD          127
#define EREMCHG         128
#define ELIBACC         129
#define ELIBBAD         130
#define ELIBSCN         131
#define ELIBMAX         132
#define ELIBEXEC        133
#define ERESTART        134
#define ESTRPIPE        135
#define EUSERS          136
#define ESOCKTNOSUPPORT 137
#define EOPNOTSUPP      138
#define EPFNOSUPPORT    139
#define ESHUTDOWN       140
#define ETOOMANYREFS    141
#define EHOSTDOWN       142
#define EUCLEAN         143
#define ENOTNAM         144
#define ENAVAIL         145
#define EISNAM          146
#define EREMOTEIO       147
#define ENOMEDIUM       148
#define EMEDIUMTYPE     149
#define ENOKEY          150
#define EKEYEXPIRED     151
#define EKEYREVOKED     152
#define EKEYREJECTED    153
#define ERFKILL         154
#define EHWPOISON       155
#define EL2NSYNC        156

// codes which musl defines as aliases

#define EWOULDBLOCK     EAGAIN
#define EDEADLOCK       EDEADLK
#define ENOTSUP         EOPNOTSUPP