/* { dg-do compile } */
/* { dg-options "-O1" } */

typedef struct {
  _Bool : 1;
} Http2Identifier;
typedef struct {
  _Bool is_end;
  Http2Identifier identifier;
} Http2DataFrame;
typedef struct {
  Http2Identifier identifier;
  _Bool end_headers;
} Http2ContinuationFrame;
typedef struct {
  union {
    Http2DataFrame data;
    Http2ContinuationFrame continuation;
  };
} Http2Frame;
typedef struct {
  Http2Identifier stream_identifier;
} Http2RawHeader;
Http2RawHeader parse_http2_continuation_frame_http2_raw_header;
Http2Frame gh2f;
void http2_send_and_receive_preface() {
  Http2ContinuationFrame continuation_frame = {
      parse_http2_continuation_frame_http2_raw_header.stream_identifier, 0};
  Http2Frame frame = {.continuation = continuation_frame};
  gh2f = frame;
}
