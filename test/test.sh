
function testcase () {
    res=`cat "$1" | json2table "$2" "$3" | sha256sum | cut -d ' ' -f 1``echo -- "$@" | base64 -w 0`
    if [[ $standard = 1 ]]; then
        echo "$res" >> answers
    else
        grep "$res" answers > /dev/null && echo -- "OK: $@" || echo -- "FAIL: $@"
    fi
}

standard=0
testcase 1.json 
testcase 1.json /
testcase 1.json /vm
testcase 1.json /vm/ip
testcase 1.json /vm/ip,password
testcase 1.json /vm,none
testcase 1.json /vm/ip/none
testcase 1.json /vm/ip,password/none
testcase 1.json /vm,none/ip
testcase 2.json /
testcase 2.json /labs
testcase 2.json /medications
testcase 2.json /labs,medications/name
testcase 2.json /medications/mineral
testcase 2.json /medications/mineral/
testcase 2.json ///////
testcase 2.json /imaging,labs/location
testcase 3.json /payload/commits,head,action
testcase 3.json /payload/commits
testcase 3.json /payload

testcase 1.json 
testcase 1.json / -p
testcase 1.json /vm -p
testcase 1.json /vm/ip -p
testcase 1.json /vm/ip,password -p
testcase 1.json /vm,none -p
testcase 1.json /vm/ip/none -p
testcase 1.json /vm/ip,password/none -p
testcase 1.json /vm,none/ip -p
testcase 2.json / -p
testcase 2.json /labs -p
testcase 2.json /medications -p
testcase 2.json /labs,medications/name -p
testcase 2.json /medications/mineral -p
testcase 2.json /medications/mineral/ -p
testcase 2.json /////// -p
testcase 2.json /imaging,labs/location -p
testcase 3.json /payload/commits,head,action -p
testcase 3.json /payload/commits -p
testcase 3.json /payload -p

