export interface AIOne {

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