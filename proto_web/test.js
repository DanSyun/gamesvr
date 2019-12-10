var pb = require("./proto");

var message = pb.PBSSMsg.create({uid: 100, webReqPlayerCharge: {
    uid: 100,
    awards: [
        {
            givingNum: 1000,
            type: pb.ENAwardType.EN_Award_Gold
        }
    ]
}});
var buff1 = pb.PBSSMsg.encode(message).finish();

var net = require('net');
var port = 40000;
var host = '47.99.116.133';
var client= new net.Socket();
client.setEncoding('binary');
client.connect(port,host,function(){
    //client.write(4+buff.length);
    for (var i = 0; i < 5; ++i)
    {
        var buffer = new Buffer(4);
        buffer.writeInt32BE(4 + buff1.length, 0);
        client.write(buffer);
        client.write(buff1);
    }
});
var count = 0;
client.on('data', function(data){
    var _data = '';
    _data += data;
    console.log('haha');
    while (_data.length >= 4)
    {
        var tmp = new Buffer(4);
        tmp.write(_data, 0, 4);
        var length;
        length = tmp.readInt32BE(0, 4);
        console.log(length);
        console.log(_data.length);
        if (_data.length < length) break;

        var buff = new Buffer(length - 4);
        buff.write(_data.slice(4), 'binary');
        var message = pb.PBSSMsg.decode(buff, buff.length);
        console.log(message);
        console.log(++count);
        _data = _data.slice(length);

        for (var i = 0; i < 3; ++i)
        {
            var buffer = new Buffer(4);
            buffer.writeInt32BE(4 + buff1.length, 0);
            // client.write(buffer);
            // client.write(buff1);
        }
    }
});
client.on('err', function(err){
    console.log(err);
});
