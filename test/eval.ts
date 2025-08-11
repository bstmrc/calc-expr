import { evalExpression } from "../src";

console.log(evalExpression("x * (saldo_total + y)", {x: 4, saldo_total: 10, y: 5}))
