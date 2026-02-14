async function init_wasm(canvas, buffer_width, buffer_height, initial, maximum) {
    const BYTES_COUNT = buffer_width * buffer_height * 4

    const memory = new WebAssembly.Memory({
        initial,
        maximum,
        shared: true
    });

    const ctx = canvas.getContext('2d');

    let imageData = new ImageData(new Uint8ClampedArray(BYTES_COUNT), buffer_width, buffer_height)

    let exports = null

    let isPlaying = true

    let animation_request = null

    let last_time = Date.now();
    let time = 0;

    canvas.pause = () => {
        isPlaying = false;
        cancelAnimationFrame(animation_request);
        canvas.dispatchEvent(new CustomEvent('pause'))
    }

    canvas.play = () => {
        last_time = Date.now()
        isPlaying = true;
        loop();
        canvas.dispatchEvent(new CustomEvent('play'))
    }

    canvas.playPause = () => {
        isPlaying = !isPlaying;
        if (!isPlaying)
            canvas.pause();
        else
            canvas.play()
    }

    canvas.isPlaying = () => {
        return isPlaying
    }

    canvas.seek = (seek_time) => {
        time = seek_time
        last_time = time
        updateCanvas(time, true)
    }

    const updateCanvas = (time, no_dispatch) => {
        exports.demo_update(time / 1000);

        const offset = exports.demo_get_buffer();

        imageData.data.set(new Uint8ClampedArray(memory.buffer, offset, BYTES_COUNT));
        ctx.putImageData(imageData, 0, 0);

        if (no_dispatch)
            return

        canvas.dispatchEvent(new CustomEvent('frame', {
            detail: {
                time
            }
        }))
    }

    const loop = () => {
        const delta = Date.now() - last_time
        last_time = Date.now();

        time += delta;
        updateCanvas(time)

        animation_request = requestAnimationFrame(loop);
    }

    const load = async () => {
        const response = await fetch('@TARGET@.wasm?' + Date.now());
        const bytes = await response.arrayBuffer();
        const {instance} = await WebAssembly.instantiate(bytes, {
            env: {
                memory,
                sinf: Math.sin,
                cosf: Math.cos,
                abs: Math.abs
            }
        });

        exports = instance.exports

        exports.demo_init()

        updateCanvas(time)
    }

    window.load_wasm = load

    async function init() {
        await load();
        loop();
    }

    await load()
    loop()
}