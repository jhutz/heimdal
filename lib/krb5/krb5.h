/* $Id$ */

#ifndef __KRB5_H__
#define __KRB5_H__

#include <sys/types.h>
#include <sys/time.h>
#include <stdarg.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_NETINET_IN6_MACHTYPES_H
#include <netinet/in6_machtypes.h>
#endif

#ifdef HAVE_SYS_BITYPES_H
#include <sys/bitypes.h>
#endif

#include <bits.h>

#include <des.h>
#include <asn1_err.h>
#include <krb5_err.h>

#include <asn1.h>

#include "config_file.h"

/* simple constants */

#ifndef TRUE
#define TRUE  1
#define FALSE 0
#endif

typedef int krb5_boolean;

typedef int32_t krb5_error_code;

typedef int krb5_kvno;

typedef u_int32_t krb5_flags;

typedef void *krb5_pointer;
typedef const void *krb5_const_pointer;

typedef octet_string krb5_data;

typedef enum krb5_cksumtype { 
  CKSUMTYPE_NONE		= 0,
  CKSUMTYPE_CRC32		= 1,
  CKSUMTYPE_RSA_MD4		= 2,
  CKSUMTYPE_RSA_MD4_DES		= 3,
  CKSUMTYPE_DES_MAC		= 4,
  CKSUMTYPE_DES_MAC_K		= 5,
  CKSUMTYPE_RSA_MD4_DES_K	= 6,
  CKSUMTYPE_RSA_MD5		= 7,
  CKSUMTYPE_RSA_MD5_DES		= 8,
  CKSUMTYPE_RSA_MD5_DES3	= 9,
  CKSUMTYPE_HMAC_SHA1_DES3	= 10
} krb5_cksumtype;



typedef enum krb5_enctype { 
  ETYPE_NULL			= 0,
  ETYPE_DES_CBC_CRC		= 1,
  ETYPE_DES_CBC_MD4		= 2,
  ETYPE_DES_CBC_MD5		= 3,
  ETYPE_DES3_CBC_MD5		= 5,
  ETYPE_DES3_CBC_SHA1		= 7,
  ETYPE_SIGN_DSA_GENERATE	= 8,
  ETYPE_ENCRYPT_RSA_PRIV	= 9,
  ETYPE_ENCRYPT_RSA_PUB		= 10,
  ETYPE_ENCTYPE_PK_CROSS	= 48
} krb5_enctype;

typedef enum krb5_preauthtype {
  KRB5_PADATA_NONE		= 0,
  KRB5_PADATA_AP_REQ,
  KRB5_PADATA_TGS_REQ		= 1,
  KRB5_PADATA_PW_SALT		= 3,
  KRB5_PADATA_ENC_TIMESTAMP	= 2,
  KRB5_PADATA_ENC_SECURID
} krb5_preauthtype;


typedef enum krb5_address_type { 
    KRB5_ADDRESS_INET = 2
} krb5_address_type;

enum {
  AP_OPTS_USE_SESSION_KEY = 1,
  AP_OPTS_MUTUAL_REQUIRED = 2
};

typedef HostAddress krb5_address;

typedef HostAddresses krb5_addresses;
#if 0
typedef struct krb5_addresses {
    int number;
    krb5_address *addrs;
} krb5_addresses;
#endif

typedef enum krb5_keytype { 
    KEYTYPE_NULL = 0,
    KEYTYPE_DES = 1 
} krb5_keytype;

typedef EncryptionKey krb5_keyblock;

struct krb5_cc_ops;

typedef struct krb5_ccache_data{
    char *residual;
    struct krb5_cc_ops *ops;
    krb5_data data;
}krb5_ccache_data;

typedef struct krb5_cc_cursor{
  int fd;
}krb5_cc_cursor;

typedef struct krb5_ccache_data *krb5_ccache;

typedef struct krb5_context_data *krb5_context;

#define USE_ASN1_PRINCIPAL
#ifdef USE_ASN1_PRINCIPAL
typedef Realm krb5_realm;
typedef Principal krb5_principal_data;
#else
typedef krb5_data krb5_realm;
typedef struct krb5_principal_data{
  int type;
  krb5_realm realm;
  krb5_data *comp;
  int ncomp;
}krb5_principal_data;

#endif
typedef krb5_principal_data *krb5_principal;
typedef const krb5_principal_data *krb5_const_principal;


typedef time_t krb5_time;

typedef struct krb5_times{
  krb5_time authtime;
  krb5_time starttime;
  krb5_time endtime;
  krb5_time renew_till;
} krb5_times;


typedef struct krb5_creds {
    krb5_principal client;
    krb5_principal server;
    krb5_keyblock session;
    krb5_times times;
    krb5_data ticket;

    krb5_data second_ticket; /* ? */
    krb5_data authdata; /* ? */
    krb5_addresses addresses;
    
} krb5_creds;

typedef struct krb5_cc_ops{
    char *prefix;
    char* (*get_name)(krb5_context, krb5_ccache);
    krb5_error_code (*resolve)(krb5_context, krb5_ccache *, const char *);
    krb5_error_code (*gen_new)(krb5_context, krb5_ccache *);
    krb5_error_code (*init)(krb5_context, krb5_ccache, krb5_principal);
    krb5_error_code (*destroy)(krb5_context, krb5_ccache);
    krb5_error_code (*close)(krb5_context, krb5_ccache);
    krb5_error_code (*store)(krb5_context, krb5_ccache, krb5_creds*);
    krb5_error_code (*retrieve)(krb5_context, krb5_ccache, 
				krb5_flags, krb5_creds*, krb5_creds);
    krb5_error_code (*get_princ)(krb5_context, krb5_ccache, krb5_principal*);
    krb5_error_code (*get_first)(krb5_context, krb5_ccache, krb5_cc_cursor *);
    krb5_error_code (*get_next)(krb5_context, krb5_ccache, 
				krb5_cc_cursor*, krb5_creds*);
    krb5_error_code (*end_get)(krb5_context, krb5_ccache, krb5_cc_cursor*);
    krb5_error_code (*remove_cred)(krb5_context, krb5_ccache, 
				   krb5_flags, krb5_creds*);
    krb5_error_code (*set_flags)(krb5_context, krb5_ccache, krb5_flags);
} krb5_cc_ops;

typedef struct krb5_context_data{
    krb5_enctype *etypes;
    char *default_realm;
    krb5_config_section *cf;
    struct error_list *et_list;
    krb5_cc_ops *cc_ops;
    int num_ops;
} krb5_context_data;

enum{
  KRB5_NT_UNKNOWNN	= 0,
  KRB5_NT_PRINCIPAL	= 1,
  KRB5_NT_SRV_INST	= 2,
  KRB5_NT_SRV_HST	= 3,
  KRB5_NT_SRV_XHST	= 4,
  KRB5_NT_UID		= 5
};


typedef struct krb5_ticket {
#if 0
    krb5_principal server;
    krb5_data enc_part;
    krb5_data enc_part2;
#endif
    EncTicketPart tkt;
    struct {
	krb5_principal client;
    } enc_part2;
} krb5_ticket;

#if 0

typedef struct krb5_authenticator_data{
    int vno;
    krb5_principal cname;
    int cusec;
    krb5_time ctime;
    int *seq_number;
    Checksum *cksum;
} krb5_authenticator_data;

#endif

typedef Authenticator krb5_authenticator_data;

typedef krb5_authenticator_data *krb5_authenticator;

typedef struct krb5_rcache{
    int dummy;
}krb5_rcache;

typedef struct krb5_fcache{
    char *filename;
}krb5_fcache;

typedef struct krb5_keytab_data {
  char *filename;
}krb5_keytab_data;

typedef struct krb5_keytab_data *krb5_keytab;

typedef struct krb5_keytab_entry {
  krb5_principal principal;
  krb5_kvno vno;
  krb5_keyblock keyblock;
} krb5_keytab_entry;

typedef struct krb5_storage{
    void *data;
    size_t (*fetch)(struct krb5_storage*, void*, size_t);
    size_t (*store)(struct krb5_storage*, void*, size_t);
    off_t (*seek)(struct krb5_storage*, off_t, int);
    void (*free)(struct krb5_storage*);
} krb5_storage;

typedef struct krb5_kt_cursor {
    int fd;
    krb5_storage *sp;
} krb5_kt_cursor;

enum {
  KRB5_AUTH_CONTEXT_DO_TIME      = 1,
  KRB5_AUTH_CONTEXT_RET_TIME     = 2,
  KRB5_AUTH_CONTEXT_DO_SEQUENCE  = 3,
  KRB5_AUTH_CONTEXT_RET_SEQUENCE = 4
};

typedef struct krb5_auth_context_data{
  int32_t flags;
  krb5_cksumtype cksumtype;
  krb5_enctype enctype;

  krb5_address local_address;
  krb5_address remote_address;
  krb5_keyblock key;
  krb5_keyblock local_subkey;
  krb5_keyblock remote_subkey;

  int32_t local_seqnumber;
  int32_t remote_seqnumber;

  krb5_authenticator authenticator;
  
  krb5_pointer i_vector;
  
  krb5_rcache rcache;
  
}krb5_auth_context_data, *krb5_auth_context;


typedef struct {
  KDC_REP part1;
  EncTGSRepPart part2;
} krb5_kdc_rep;

krb5_error_code
krb5_init_context(krb5_context *context);

krb5_error_code
krb5_auth_con_init(krb5_context context,
		   krb5_auth_context *auth_context);

krb5_error_code
krb5_auth_con_free(krb5_context context,
		   krb5_auth_context auth_context);

krb5_error_code
krb5_auth_con_setflags(krb5_context context,
		       krb5_auth_context auth_context,
		       int32_t flags);

krb5_error_code
krb5_auth_con_getflags(krb5_context context,
		       krb5_auth_context auth_context,
		       int32_t *flags);

krb5_error_code
krb5_auth_con_setaddrs(krb5_context context,
		       krb5_auth_context auth_context,
		       krb5_address *local_addr,
		       krb5_address *remote_addr);

krb5_error_code
krb5_auth_con_getaddrs(krb5_context context,
		       krb5_auth_context auth_context,
		       krb5_address **local_addr,
		       krb5_address **remote_addr);

krb5_error_code
krb5_auth_con_setuserkey(krb5_context context,
			 krb5_auth_context auth_context,
			 krb5_keyblock *keyblock);

krb5_error_code
krb5_auth_con_getkey(krb5_context context,
		     krb5_auth_context auth_context,
		     krb5_keyblock **keyblock);

krb5_error_code
krb5_auth_con_getlocalsubkey(krb5_context context,
			     krb5_auth_context auth_context,
			     krb5_keyblock **keyblock);

krb5_error_code
krb5_auth_con_getremotesubkey(krb5_context context,
			      krb5_auth_context auth_context,
			      krb5_keyblock **keyblock);

void
krb5_free_keyblock(krb5_context context,
		   krb5_keyblock *keyblock);

krb5_error_code
krb5_auth_setcksumtype(krb5_context context,
		       krb5_auth_context auth_context,
		       krb5_cksumtype cksumtype);

krb5_error_code
krb5_auth_getcksumtype(krb5_context context,
		       krb5_auth_context auth_context,
		       krb5_cksumtype *cksumtype);

krb5_error_code
krb5_auth_getlocalseqnumber(krb5_context context,
			    krb5_auth_context auth_context,
			    int32_t *seqnumber);

krb5_error_code
krb5_auth_getremoteseqnumber(krb5_context context,
			     krb5_auth_context auth_context,
			     int32_t *seqnumber);

krb5_error_code
krb5_auth_setlocalseqnumber(krb5_context context,
			    krb5_auth_context auth_context,
			    int32_t seqnumber);

krb5_error_code
krb5_auth_setremoteseqnumber(krb5_context context,
			     krb5_auth_context auth_context,
			     int32_t seqnumber);

krb5_error_code
krb5_auth_getauthenticator(krb5_context context,
			   krb5_auth_context auth_context,
			   krb5_authenticator *authenticator);

#if 0
krb5_boolean
krb5_address_search(krb5_context context,
		    const krb5_address *addr,
		    krb5_address *const *addrlist);
#endif

krb5_boolean
krb5_address_search(krb5_context context,
		    const krb5_address *addr,
		    const krb5_addresses *addrlist);

krb5_boolean
krb5_address_compare(krb5_context context,
		     const krb5_address *addr1,
		     const krb5_address *addr2);

int
krb5_address_order(krb5_context context,
		   const krb5_address *addr1,
		   const krb5_address *addr2);

krb5_error_code
krb5_copy_addresses(krb5_context context,
		    const krb5_addresses *inaddr,
		    krb5_addresses *outaddr);

krb5_error_code
krb5_free_address(krb5_context context,
		  krb5_address *address);

krb5_error_code
krb5_free_addresses(krb5_context context,
		    krb5_addresses *addresses);

void
krb5_free_authenticator(krb5_context,
			krb5_authenticator *authenticator);

krb5_error_code
krb5_auth_initvector(krb5_context context,
		     krb5_auth_context auth_context);

krb5_error_code
krb5_set_initvector(krb5_context context,
		    krb5_auth_context auth_context,
		    krb5_pointer ivector);

krb5_error_code
krb5_set_rcache(krb5_context context,
		krb5_auth_context auth_context,
		krb5_rcache rcache);

krb5_error_code
krb5_get_cred_from_kdc(krb5_context,
		       krb5_ccache ccache,
		       krb5_creds *in_cred,
		       krb5_creds **out_cred,
		       krb5_creds **tgts);


krb5_error_code
krb5_get_credentials(krb5_context context,
		     krb5_flags options,
		     krb5_ccache ccache,
		     krb5_creds *in_creds,
		     krb5_creds **out_creds);

typedef krb5_error_code (*krb5_key_proc)(krb5_context context,
					 krb5_keytype type,
					 krb5_data *salt,
					 krb5_const_pointer keyseed,
					 krb5_keyblock **key);
typedef krb5_error_code (*krb5_decrypt_proc)(krb5_context context,
					     const krb5_keyblock *key,
					     krb5_const_pointer decrypt_arg,
					     krb5_kdc_rep *dec_rep);

krb5_error_code
krb5_get_in_tkt(krb5_context context,
		krb5_flags options,
		const krb5_addresses *addrs,
/*		krb5_address *const *addrs,*/
		const krb5_enctype *etypes,
		const krb5_preauthtype *ptypes,
		krb5_key_proc key_proc,
		krb5_const_pointer keyseed,
		krb5_decrypt_proc decrypt_proc,
		krb5_const_pointer decryptarg,
		krb5_creds *creds,
		krb5_ccache ccache,
		krb5_kdc_rep **ret_as_reply);

krb5_error_code
krb5_get_in_tkt_with_password (krb5_context context,
			       krb5_flags options,
			       krb5_addresses *addrs,
/*			       krb5_address *const *addrs,*/
			       const krb5_enctype *etypes,
			       const krb5_preauthtype *pre_auth_types,
			       const char *password,
			       krb5_ccache ccache,
			       krb5_creds *creds,
			       krb5_kdc_rep **ret_as_reply);

krb5_error_code
krb5_mk_error(krb5_principal princ, 
	      krb5_error_code error_code,
	      char *e_text,
	      krb5_data *e_data,
	      krb5_data *err);

krb5_error_code
krb5_mk_req(krb5_context context,
	    krb5_auth_context *auth_context,
	    const krb5_flags ap_req_options,
	    char *service,
	    char *hostname,
	    krb5_data *in_data,
	    krb5_ccache ccache,
	    krb5_data *outbuf);

krb5_error_code
krb5_mk_req_extended(krb5_context context,
		     krb5_auth_context *auth_context,
		     const krb5_flags ap_req_options,
		     krb5_data *in_data,
		     krb5_creds *in_creds,
		     krb5_data *outbuf);

krb5_error_code
krb5_mk_rep(krb5_context context,
	    krb5_auth_context *auth_context,
	    krb5_data *outbuf);

krb5_error_code
krb5_generate_subkey(krb5_context context,
		     const krb5_keyblock *key,
		     krb5_keyblock **subkey);

krb5_error_code
krb5_generate_seq_number(krb5_context context,
			 const krb5_keyblock *key,
			 int32_t *seqno);

krb5_error_code
krb5_rd_req(krb5_context context,
	    krb5_auth_context *auth_context,
	    const krb5_data *inbuf,
	    krb5_const_principal server,
	    krb5_keytab keytab,
	    krb5_flags *ap_req_options,
	    krb5_ticket **ticket);

krb5_error_code
krb5_rd_req_with_keyblock(krb5_context context,
			  krb5_auth_context *auth_context,
			  const krb5_data *inbuf,
			  krb5_const_principal server,
			  krb5_keyblock *keyblock,
			  krb5_flags *ap_req_options,
			  krb5_ticket **ticket);


typedef EncAPRepPart krb5_ap_rep_enc_part;

krb5_error_code
krb5_rd_rep(krb5_context context,
	    krb5_auth_context auth_context,
	    const krb5_data *inbuf,
	    krb5_ap_rep_enc_part **repl);

void
krb5_free_ap_rep_enc_part (krb5_context context,
			   krb5_ap_rep_enc_part *val);

krb5_error_code
krb5_mk_safe(krb5_context context,
	     krb5_auth_context auth_context,
	     const krb5_data *userdata,
	     krb5_data *outbuf,
	     /*krb5_replay_data*/ void *outdata);

krb5_error_code
krb5_rd_safe(krb5_context context,
	     krb5_auth_context auth_context,
	     const krb5_data *inbuf,
	     krb5_data *outbuf,
	     /*krb5_replay_data*/ void *outdata);

krb5_error_code
krb5_mk_priv(krb5_context context,
	     krb5_auth_context auth_context,
	     const krb5_data *userdata,
	     krb5_data *outbuf,
	     /*krb5_replay_data*/ void *outdata);

krb5_error_code
krb5_rd_priv(krb5_context context,
	     krb5_auth_context auth_context,
	     const krb5_data *inbuf,
	     krb5_data *outbuf,
	     /*krb5_replay_data*/ void *outdata);

krb5_error_code
krb5_sendauth(krb5_context context,
	      krb5_auth_context *auth_context,
	      krb5_pointer fd,
	      char *appl_version,
	      krb5_principal client,
	      krb5_principal server,
	      krb5_flags ap_req_options,
	      krb5_data *in_data,
	      krb5_creds *in_creds,
	      krb5_ccache ccache,
	      /*krb5_error*/ void **error,
	      krb5_ap_rep_enc_part **rep_result,
	      krb5_creds ** out_creds);

#define KRB5_RECVAUTH_IGNORE_VERSION 1

krb5_error_code
krb5_recvauth(krb5_context context,
	      krb5_auth_context *auth_context,
	      krb5_pointer fd,
	      char *appl_version,
	      krb5_principal server,
	      int32_t flags,
	      krb5_keytab keytab,
	      krb5_ticket **ticket);

#define KRB5_SENDAUTH_VERSION "KRB5_SENDAUTH_V1.0"

krb5_error_code
krb5_parse_name(krb5_context context,
		const char *name,
		krb5_principal *principal);

void
krb5_free_principal(krb5_context context,
		    krb5_principal principal);

krb5_error_code
krb5_unparse_name(krb5_context context,
		  krb5_principal principal,
		  char **name);

krb5_error_code
krb5_unparse_name_ext(krb5_context context,
		      krb5_const_principal principal,
		      char **name,
		      size_t *size);

krb5_realm*
krb5_princ_realm(krb5_context context,
		 krb5_principal principal);

void
krb5_princ_set_realm(krb5_context context,
		     krb5_principal principal,
		     krb5_realm *realm);

krb5_error_code
krb5_principal_set_component(krb5_context, 
			     krb5_principal, 
			     int, 
			     void*, 
			     size_t);

krb5_error_code
krb5_build_principal(krb5_context context,
		     krb5_principal *principal,
		     int rlen,
		     const char *realm,
		     ...);

krb5_error_code
krb5_build_principal_va(krb5_context context,
			krb5_principal *principal,
			int rlen,
			const char *realm,
			va_list ap);

krb5_error_code
krb5_build_principal_ext(krb5_context context,
			 krb5_principal *principal,
			 int rlen,
			 const char *realm,
			 ...);

krb5_error_code
krb5_copy_principal(krb5_context context,
		    krb5_const_principal inprinc,
		    krb5_principal *outprinc);

krb5_boolean
krb5_principal_compare(krb5_context context,
		       krb5_const_principal princ1,
		       krb5_const_principal princ2);

krb5_boolean
krb5_realm_compare(krb5_context context,
		   krb5_const_principal princ1,
		   krb5_const_principal princ2);
		   
krb5_error_code
krb5_425_conv_principal(krb5_context context,
			const char *name,
			const char *instance,
			const char *realm,
			krb5_principal *princ);

krb5_error_code
krb5_sname_to_principal (krb5_context context,
			 const char *hostname,
			 const char *sname,
			 int32_t type,
			 krb5_principal *ret_princ);

krb5_error_code
krb5_get_krbhst (krb5_context context,
		 const krb5_realm *realm,
		 char ***hostlist);

krb5_error_code
krb5_free_krbhst (krb5_context context,
		  char *const *hostlist);

krb5_error_code
krb5_get_host_realm(krb5_context context,
		    const char *host,
		    char ***realms);

krb5_boolean
krb5_kuserok (krb5_context context,
	      krb5_principal principal,
	      const char *luser);

int
krb5_verify_user(krb5_context context, 
		 krb5_principal principal,
		 krb5_ccache ccache,
		 const char *password,
		 int secure,
		 const char *service);

/* variables */

extern const char krb5_config_file[];
extern const char krb5_defkeyname[];

void krb5_free_context(krb5_context context);

krb5_error_code
krb5_get_all_client_addrs (krb5_addresses *res);

krb5_error_code
krb5_set_default_in_tkt_etypes(krb5_context context, 
			       const krb5_enctype *etypes);
krb5_error_code
krb5_get_default_in_tkt_etypes(krb5_context context,
			       krb5_enctype **etypes);


krb5_error_code
krb5_string_to_key (char *str,
		    krb5_data *salt,
		    krb5_keyblock *key);

krb5_error_code
krb5_get_salt (krb5_principal princ,
	       krb5_data *salt);

ssize_t
krb5_net_read (krb5_context context,
	       int fd,
	       void *buf,
	       size_t len);

ssize_t
krb5_net_write (krb5_context context,
		int fd,
		const void *buf,
		size_t len);

krb5_error_code
krb5_encrypt (krb5_context context,
	      void *ptr,
	      size_t len,
	      int etype,
	      const krb5_keyblock *keyblock,
	      krb5_data *result);

krb5_error_code
krb5_encrypt_EncryptedData(krb5_context, void*, size_t, int,
			   const krb5_keyblock*, EncryptedData*);

krb5_error_code
krb5_decrypt (krb5_context context,
	      void *ptr,
	      size_t len,
	      int etype,
	      const krb5_keyblock *keyblock,
	      krb5_data *result);

krb5_error_code
krb5_generate_random_keyblock(krb5_context,
			      int,
			      krb5_keyblock*);

krb5_error_code
krb5_etype2keytype(krb5_context,
		   krb5_enctype,
		   krb5_keytype*);

krb5_error_code
krb5_create_checksum (krb5_context context,
		      krb5_cksumtype type,
		      void *ptr,
		      size_t len,
		      Checksum *result);

krb5_error_code
krb5_verify_checksum (krb5_context context,
		      void *ptr,
		      size_t len,
		      Checksum *sum);

krb5_error_code
krb5_cksumsize(krb5_context,
	       krb5_cksumtype,
	       size_t*);


#include "cache.h"

#include "keytab.h"

struct error_table {
    char const * const * msgs;
    long base;
    int n_msgs;
};

struct error_list {
    struct error_list *next;
    const struct error_table * table;
};

const char *krb5_get_err_text(krb5_context context, long code);

void krb5_init_ets(krb5_context context);

/* XXX these are glue functions and really don't belong here */

krb5_error_code krb5_principal2principalname (PrincipalName *p, 
					      krb5_principal from);
krb5_error_code principalname2krb5_principal (krb5_principal *principal,
					      PrincipalName from,
					      char *realm);

#include "store.h"

#endif /* __KRB5_H__ */

