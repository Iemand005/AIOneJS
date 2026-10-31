interface AIOneNative {
    /** Returns a static greeting; useful as a load smoke test. */
    hello(): string;

    /**
     * Loads a GGUF model and offloads all layers to the preferred GPU
     * (falls back to iGPU/CPU when no GPU is available). Blocks until loaded.
     */
    loadModel(path: string): void;

    /**
     * Runs a synchronous completion against the loaded model.
     */
    generate(prompt: string, maxTokens?: number): string;

    /** Unloads the model and frees its VRAM. */
    unloadModel(): void;
}

declare const AIOne: AIOneNative;
export = AIOne;