export interface AIOneOptions {
    workerUrl?: string;
    name?: string;
}

export default class AIOne {
    static create(options?: AIOneOptions): Promise<AIOne>;

    loadModel(file: File): Promise<void>;
    generate(prompt: string, maxTokens?: number): Promise<string>;
    unloadModel(): Promise<void>;
    close(): void;
}