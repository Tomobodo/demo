const ws = new WebSocket('ws://localhost:8081');
ws.onmessage = (e) => {
    if (e.data === 'build') {
        console.log('New wasm build, reloading...');
        load_wasm();
    }

    if (e.data === 'shell') {
        console.log('New shell, refreshing...')
        window.location.reload()
    }
}