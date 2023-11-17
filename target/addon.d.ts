

export interface ShmCacheStat{
    max_size: number,
    total_size: number,
    free_size: number,
    used_size: number,
    grow_count: number,
    key_cont: number,
    last_clean_time: number,
    version: number
}

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
    public stat(): ShmCacheStat;
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
    stat(): ShmCacheStat;
    static setMaxAge(maxAge: number): void;
}

export function hello(): void;


