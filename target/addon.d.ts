

export class NodeShareCache {
    constructor(name: string, size: number, renew: boolean);
    public setMaxAge(maxAge: number): void;
    public setMaxSize(maxAge: number): void;
    public setLock(lock: boolean): void;
    public setLogLevel(level: number): void;
    public get(key: string): string | undefined;
    public set(key: string, value: string): void;
    public del(key: string): void;
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
    get(key:string): string | undefined;
    del(key:string): void;
    set(key:string, value:string,maxAge:number): void
    setMaxAge(maxAge: number): void;
    destroy(): void;
    static setMaxAge(maxAge: number): void;
}

export function hello(): void;


