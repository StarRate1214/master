룰 관련 소스코드

NonPayload 미구현 옵션
    - id
    - ipopts
    - flow
    - flowbits
    - icmp_id
    - icmp_seq
    - rpc
    - ip_proto
    - stream_reassemble
    - stream_size
    - sameip

Compare.cpp에서 룰 비교를 전부해줘야 되나 or 일치하는게 나오면 바로 끝내야되나
    - (일치하는게 나오면 끝날때) 룰에 우선순위 부여
DB사용을 강제하게 할지 말지