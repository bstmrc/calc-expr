import { Binder } from "./binder";

export const evalExpression = (
  expr: string,
  vars: Record<string, number>
): number  => {
  const binder = Binder.getInstance();
  const varsString = Object.entries(vars)
    .map(([key, value]) => `${key}=${value}`)
    .join(",");

  const libparser = binder.getLibrary();

  const result = libparser.evalExpression(expr, varsString);
  return result;
}