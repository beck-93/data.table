#include<stdint.h>

/*
 * a struct to carry out results of embarrassingly parallel computation
 * catch verbose stdout messages, stderr messages, warnings and errors
 * safe to use inside parallel regions, of course allocated outside
 */
#define ANS_MSG_SIZE 4096
typedef struct ans_t {
  int32_t *int_v;        // used in nafill
  double *dbl_v;         // used in froll, nafill
  int64_t *int64_v;      // not used yet
  uint8_t status;        // 0:ok, 1:message, 2:warning, 3:error; unix return signal: {0,1,2}=0, {3}=1
  char message[4][ANS_MSG_SIZE]; // STDOUT: output, STDERR: message, warning, error
// implicit n_message limit discussed here: https://github.com/Rdatatable/data.table/issues/3423#issuecomment-487722586
} ans_t;

/*
 * data of a data.table to be accessible from parallel region
 */
typedef union { // all const is fine for merge
  const int *i; // logical and int
  const double *d;
  const SEXP *s; // string and list
  const Rcomplex *c;
  const Rbyte *b;
} column_t;
typedef struct dt_t { // could store pointer to SEXP itself, indices, sorted info, names, classes, alloccol
  unsigned int ncol;
  unsigned int nrow;
  SEXPTYPE *types;
  bool *int64;
  column_t *cols;
} dt_t;
