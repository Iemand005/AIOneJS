
let session;

const moduleUrl = new URL(
    "/build/emscripten-webgpu/AIOneWeb.js",
    import.meta.url
).href;

self.onmessage = async ({ data }) => {
    try {
        if (data.type === "load") {
            const { default: createModule } = await import(moduleUrl);

            const Module = await createModule({
				workerFile: data.file,
				locateFile: path =>
					new URL(path, moduleUrl).href
			});

			session = new Module.ModelSession();
			session.loadModel("/models/" + data.file.name);

            self.postMessage({ type: "loaded" });
        }

        if (data.type === "generate") {
            if (!session || !session.isLoaded()) {
                throw new Error("No model loaded");
            }

            const result = session.generate(
                data.prompt,
                data.maxTokens ?? 128
            );

            self.postMessage({
                type: "generated",
                result
            });
        }

        if (data.type === "unload") {
            session?.unloadModel();
            session = undefined;

            self.postMessage({ type: "unloaded" });
        }
    } catch (error) {
        self.postMessage({
            type: "error",
            message: error.message
        });
    }
};
