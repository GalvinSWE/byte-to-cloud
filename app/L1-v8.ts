// ============================================================
// L1 · V8: how do you make JavaScript SLOWER without changing the algorithm?
// Run:
//   node --trace-opt --trace-deopt app/L1-v8.ts jit 2>&1 | grep -E "add|---|Nguy"
//   node app/L1-v8.ts shape
//   node --trace-gc app/L1-v8.ts gc 2>&1 | head -5
//   node --trace-gc app/L1-v8.ts gc 2>&1 | grep Mark-Compact
// --trace-opt / --trace-deopt / --trace-gc are V8 flags. They print when V8
// optimizes a function, deoptimizes it, and collects garbage.
// ============================================================

const mode = process.argv[2]; // argv = argument vector: the word typed after the file name (jit, shape, gc)

// ---------- Experiment 1: JIT and deoptimization ----------
// `any` turns off TypeScript's type checks on purpose, so we can pass numbers AND strings.
function add(a: any, b: any) {
  return a + b;
}

if (mode === 'jit') {
  let sum = 0;
  for (let i = 0; i < 200_000; i++) sum = add(sum, 1); // call add 200,000 times, ALWAYS with small integers
  console.log('--- now passing strings ---');
  console.log(add('Nguyễn ', 'Văn An'));                 // break V8's assumption "a and b are always small integers"
  console.log('sum =', sum);
}

// ---------- Experiment 2: hidden classes (object shapes) ----------
if (mode === 'shape') {
  function getX(o: any) {
    return o.x; // V8 remembers "for this shape, x lives in slot N" (an inline cache)
  }
  // 1 million objects with ONE shape: {x, y}
  const same = Array.from({ length: 1_000_000 }, (_, i) => ({ x: i, y: i }));
  // 1 million objects with SIX different shapes (different property names, or a different order)
  const mixed = Array.from({ length: 1_000_000 }, (_, i) => {
    switch (i % 6) {
      case 0: return { x: i, y: i };
      case 1: return { y: i, x: i };       // same names, DIFFERENT ORDER = a different shape
      case 2: return { x: i, z: i };
      case 3: return { a: 1, x: i };
      case 4: return { x: i, y: i, z: i };
      default: return { b: 1, c: 2, x: i };
    }
  });
  for (const [label, list] of [['same shape', same], ['6 shapes', mixed]] as const) {
    let total = 0;
    const t = performance.now();                                   // performance.now(): a precise clock, in milliseconds
    for (let round = 0; round < 20; round++) for (const o of list) total += getX(o);
    console.log(`${label.padEnd(10)}: ${(performance.now() - t).toFixed(0)} ms`);
  }
}

// ---------- Experiment 3: generational garbage collection ----------
if (mode === 'gc') {
  const keep: object[] = [];
  for (let i = 0; i < 3_000_000; i++) {
    const tmp = { id: i, name: 'booking-' + i }; // create 3 million objects
    if (i % 10 === 0) keep.push(tmp);            // keep only 1 in 10; the other 9 become garbage immediately
  }
  console.log('kept', keep.length, 'objects');
}
