

export class NodeShareCache {
    constructor(name: string, size: number, renew: boolean);
    public setMaxAge(maxAge: number): void;
    public setMaxSize(maxAge: number): void;
    public setLock(lock: boolean): void;
    public setLogLevel(level: number): void;
    public get(key: string): string | undefined;
    public set(key: string, value: string, maxAge?: number): void;
    public destroy(): void;
}

interface ShmCacheConfig {
    name?: string;
    size?: number;
    maxAge?: number;
    maxSize?: number
}
export class ShmCache {
    cache: NodeShareCache;
    constructor(config: ShmCacheConfig);
    get(key): string | undefined
    set(key, value): void
    setMaxAge(maxAge: number): void;
    static setMaxAge(maxAge: number): void;
}

export function hello(): void;


