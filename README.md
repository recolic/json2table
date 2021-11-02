# json2table

Convert huge annoying json to table. 

Build it with `make`

## Usage

```
cat huge.json | json2table /all/users
curl -s 'https://config.edge.skype.net/annoying-json-api' | json2table /M365FleetAGC/AG98/Buildout/Stage0/AzureResources/KeyVault/TorusKeyVault
```

## Why use this tool

Because I'm silly and naive. For example, you have this json: 

```
{
  "vm": [
    {"username": "he,llo", "ip": "1.1.1.1", "password": "n,oob"}, 
    {"username": "hello", "ip": "1.1.1.1", "password": "b"}, 
    {"username": "hell,o4", "ip": "12.1.1.1", "password": "noob"}, 
    {"username": "hello", "ip": "1.1.1.1", "password": "oob"}, 
    {"username": "hel,lo4", "ip": "1.21.1.1", "password": "noob"}, 
    {"username": "hello", "ip": "1.1.21.1", "password": "no,,ob"}, 
    {"username": "hello", "ip": "1.1.21.1", "password": "noob"}, 
    {"username": "he,llo4", "ip": "1.1.21.1", "password": "noo2b"}, 
    {"username": "hello", "ip": "1.12.12.1", "password": "no2,,,ob"}, 
    {"username": "he,llo5", "ip": "1.21.1.1", "password": "noob"}, 
    {"username": "hello", "ip": "1.1.1.1", "password": "noob"}, 
    {"username": "hello3", "ip": "1.12.1.1", "password": "no222ob"}, 
    {"username": "hello", "ip": "1.1.1.1", "password": "noo,,,22b"}, 
    {"username": "hello", "ip": "1.1.12.1", "password": "noob"}, 
    {"username": "hello1", "ip": "1.1.1.1", "password": "no2ob"}, 
    {"username": "he,llo", "ip": "1.12.1.1", "password": "n2oo,,b"}, 
    {"username": "hello1", "ip": "1.1.1.1", "password": "noo2b"}, 
    {"username": "hello", "ip": "1.12.1.1", "password": "noo22b"}, 
    {"username": "he,llo", "ip": "1.12.21.1", "password": "no22o,,b"}, 
    {"username": "hello", "ip": "1.1.1.21", "password": "noob"}, 
    {"username": "hel,lo1", "ip": "1.1.1.21", "password": "no2ob"}, 
    {"username": "he,llo11", "ip": "1.1.1.21", "password": "2noob,,,"}
  ]
}
```

How could you get the table easily in commandline? No way! That's why I created this silly tool to fuck it. 

Another example, you have this json, it looks like a table: 

```
[{
  "users": [{"nobody":[[{"shit1":1, "shit2":2, "shit3":3}]]}]
}]
```

If you use jq or other tools, you have to write `json[0]['users'][0]["nobody"][0][0]["shit1"]`, but you can write `json/users/nobody/shit1` here. 

## WARNING

**The code in this repo is very very silly, it's completely a mess! I have warned you, DO NOT try to read or fix or understand the code.** 

I even didn't write a document for it. Then you know how silly it is! 

