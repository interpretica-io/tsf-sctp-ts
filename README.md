# tsf-sctp-ts

A Test Environment suite that exercises
[tsf-sctp](https://github.com/interpretica-io/tsf-sctp) (`tapi_sctp`)
against an SCTP peer, from the agent it runs on.

| Test | What it checks |
|---|---|
| `probe` | opens an SCTP association to `TSF_SCTP_HOST:TSF_SCTP_PORT`, logs the negotiated state / in-out stream counts / path MTU / multihoming addresses, and asserts the association is established with non-zero streams |

The association runs on the agent, in its RPC server, over the Linux
lksctp sockets API — read-only (connect, read status, close). **The peer
is the host's to supply**: with no `TSF_SCTP_HOST`/`TSF_SCTP_PORT` set,
`probe` skips cleanly (a bare run is green). Point it at a SIGTRAN/NGAP
endpoint or any `sctp`-listening service to exercise it.

## Running it

Needs Docker and `test-environment` as a sibling directory:

```bash
TSF_SCTP_HOST=10.0.0.1 TSF_SCTP_PORT=36412 ./scripts/run.sh docker guess --cfg=localhost
```

The agent host needs **libsctp / lksctp-tools** (`apt install libsctp-dev`),
already in the suite's Dockerfile. Run natively (omit `docker`) against a
host named in `conf/rcf.conf`.

## Status

Not yet asserted green here; verified by building and running natively
on the lab agent. The lksctp API usage in tsf-sctp was written against
lksctp-tools 1.0.19.
