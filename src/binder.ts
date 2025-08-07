import os from "os"
import path from "path"
import { libname } from "./utils/libnames.map"

export class Binder {
  private static instance: Binder
  private readonly libname: string
  private addon: any
  private constructor(
    private readonly platform: NodeJS.Platform = os.platform(),
    private readonly arch: string = os.arch(),
  ) {
    this.libname = libname(this.platform);
  }

  public static getInstance(): Binder {
    if (!Binder.instance) {
      Binder.instance = new Binder();
    }
    return Binder.instance;
  }

  private setAddon(addon: any) {
    this.addon = addon;
  }

  private prepare() {
    if (!this.libname) {
      throw new Error(`Unsupported platform: ${this.platform}`);
    }

    const libPath = path.join(__dirname, "..", "addon", "libparser", `${this.platform}-${this.arch}`, this.libname);

    const addon = require("bindings")("evalExpr")
    addon.load(libPath)

    return addon;
  }

  public getLibrary()  {
    if (this.addon) {
      return this.addon;
    }

    const addon = this.prepare();

    if (!addon) {
      throw new Error(`Failed to load library: ${this.libname}`);
    }
    this.setAddon(addon);

    return this.addon;
  }
}