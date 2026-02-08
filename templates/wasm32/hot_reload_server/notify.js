const {WebSocketServer} = require('ws')
const http = require('http')

const wss = new WebSocketServer({port: 8081})

const server = http.createServer((req, res) => {
    wss.clients?.forEach(client => {
        if (client.readyState === 1)
            client.send('reload')
    });

    res.writeHead(200, {'Content-Type': 'text/plain'})
    res.end('OK');
})

server.listen(8082, () => {
    console.log("curl http trigger")
});