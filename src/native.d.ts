declare module "*.node" {
	interface AIOneNative {
		hello(): string;
		loadModel(path: string): Model;
	}

	const AIOne: AIOneNative;
	export = AIOne;
}