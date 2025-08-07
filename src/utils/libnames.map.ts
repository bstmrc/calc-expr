const libNameMap: Record<string, string> = {
  win32: "lib.dll",
  linux: "lib.so"
};

export const libname = (platform: string) => {
  return libNameMap[platform] || libNameMap["linux"]
}