# calc-expr

Analizador Pratt construido en Go para el calculo de expresiones matemáticas simples.

## Características

- Ejecución de expresiones matemáticas infijas simples.
- Analizador léxico a partir del análisis de Pratt.
- Poder de enlace de los operadores.
- Los parentesis tienen prioridad de precedencia.
- Definición de variables sobre la expresión.
- Core construido en Go

## Instalación

```bash
npm install calc-expr

// or

pnpm add calc-expr
```

## Uso

### 1. Consumir la función desde Node.js
```ts
import { evalExpression } from 'calc-expr';

const result = evalExpression('var_1 + x + 20 * (15 + 2)', {var_1: 2, x: 3}) // returns: 345
```

# API

| Clase                        | Descripción                                                |
| ---------------------------- | ---------------------------------------------------------- |
| `evalExpression(expr: string, vars: {var1: number, varN: number}): number`              | Ejecuta el cálculo de la expresión bindeando los valores de sus variables |