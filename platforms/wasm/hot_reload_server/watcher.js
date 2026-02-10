import chokidar from 'chokidar'
import { WebSocketServer } from 'ws'
import { spawn, exec } from 'child_process'
import express from 'express'

const WEB_SERVER_PORT = 8080
const WEB_SOCKET_PORT = 8081
const BUILD_PATH = "build/wasm/Debug/dist"

// http server

const app = express()

app.use((req, res, next) => {
    res.setHeader('Cross-Origin-Opener-Policy', 'same-origin');
    res.setHeader('Cross-Origin-Embedder-Policy', 'require-corp');
    next();
})

app.use(express.static(BUILD_PATH));

app.listen(WEB_SERVER_PORT, () => {
    console.log(`🚀 Server ready on http://localhost:${WEB_SERVER_PORT}`);
});

// socket server

const wss = new WebSocketServer({
    port: WEB_SOCKET_PORT
})

function notify_new_build() {
    wss.clients?.forEach(client => {
        if (client.readyState === 1)
            client.send('build')
    });
}

function notify_new_shell() {
    wss.clients?.forEach(client => {
        if (client.readyState === 1)
            client.send('shell')
    })
}

// watchers

const source_watcher = chokidar.watch('src', {
    ignored: /(^|[\/\\])\../,
    ignoreInitial: true,
    depth: 99
})

const shell_watcher = chokidar.watch('platforms/wasm/shell', {
    ignored: /(^|[\/\\])\../,
    ignoreInitial: true,
    depth: 99
})

console.log("The watcher is watching...")

// watch changes in shell to reload
source_watcher.on('change', build)


shell_watcher.on('change', build_shell)

function build(path) {
    console.log("Source file changed: ", path)

    const preset_name = "Debug_wasm"

    const build = spawn('cmake', ['--build', '--preset', preset_name], { stdio: 'inherit'})

    build.on('close', (code) => {
        if (code === 0) {
            notify_new_build()
        }
    })
}

function build_shell (path) {
    console.log("Shell file changed: ", path)

    const preset_name = "Debug_wasm"

    const configure = spawn('cmake',['--preset', preset_name], { stdio: 'inherit'})

    configure.on('close', code => {
        if (code === 0) {
            notify_new_shell()
        }
    })
}

// open browser

function openBrowser(url) {
    const start = process.platform === 'darwin' ? 'open' :
        process.platform === 'win32' ? 'start' :
            'xdg-open';

    const command = process.platform === 'win32' ? `${start} "" "${url}"` : `${start} "${url}"`;

    exec(command, (err) => {
        if (err) {
            console.error(`Impossible d'ouvrir le navigateur : ${err.message}`);
        }
    });
}

openBrowser(`http://localhost:${WEB_SERVER_PORT}`);