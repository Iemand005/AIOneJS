export interface AIOne {
    loadModel(path: string): void | Promise<void>;

    generate(prompt: string, maxTokens?: number): string | Promise<string>;

    unloadModel(): void | Promise<void>;
}