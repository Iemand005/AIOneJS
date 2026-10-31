declare module "*.node" {
	interface AIOneNative {
		hello(): string;
	}

	const AIOne: AIOneNative;
	export = AIOne;
}