/** @file
 * @brief SCTP Group
 *
 * Open an SCTP association to a peer and read what it negotiated. The
 * peer is the test's to supply (env TSF_SCTP_HOST, TSF_SCTP_PORT); with
 * none configured the test skips cleanly - a host with no SCTP peer to
 * point at is not a failure. SCTP carries the telecom signalling stacks
 * (SIGTRAN/M3UA, Diameter, S1AP/NGAP) that TE's TCP/UDP cannot.
 *
 * Copyright (C) 2026 Interpretica Unipessoal Lda
 */

#define TE_TEST_NAME    "sctp/probe"

#include "te_config.h"
#include <stdlib.h>
#include "tapi_test.h"
#include "te_string.h"
#include "te_vector.h"

#include "tapi_sctp.h"
#include "tsapi_sctp.h"

int
main(int argc, char **argv)
{
    tsapi_sctp_session sess = {0};
    tapi_sctp_assoc assoc;
    bool assoc_ready = false;
    const char *host;
    const char *port_s;
    int port;

    TEST_START;

    host = getenv("TSF_SCTP_HOST");
    port_s = getenv("TSF_SCTP_PORT");
    port = (port_s != NULL && port_s[0] != '\0') ? atoi(port_s) : 0;
    if (host == NULL || host[0] == '\0' || port <= 0)
        TEST_SKIP("Set TSF_SCTP_HOST and TSF_SCTP_PORT to point at an "
                  "SCTP peer (e.g. a SIGTRAN/NGAP endpoint)");

    TEST_STEP("Open a session to the agent");
    CHECK_RC(tsapi_sctp_session_init(&sess, "pco_sctp_probe"));

    TEST_STEP("Open an SCTP association to %s:%d", host, port);
    CHECK_RC(tapi_sctp_probe(sess.pco, host, port, 5000, &assoc));
    assoc_ready = true;
    tapi_sctp_assoc_log(&assoc);

    TEST_STEP("The association is established and self-consistent");
    if (!tapi_sctp_established(&assoc))
        TEST_VERDICT("no SCTP association established to %s:%d", host, port);
    if (assoc.in_streams == 0 || assoc.out_streams == 0)
        TEST_VERDICT("association reports zero streams (in=%u out=%u)",
                     assoc.in_streams, assoc.out_streams);
    RING("SCTP %s:%d established: %u in / %u out streams, %u peer path(s)",
         host, port, assoc.in_streams, assoc.out_streams,
         (unsigned)te_vec_size(&assoc.peer_addrs));

    TEST_SUCCESS;

cleanup:
    if (assoc_ready)
        tapi_sctp_assoc_free(&assoc);
    tsapi_sctp_session_fini(&sess);
    TEST_END;
}
