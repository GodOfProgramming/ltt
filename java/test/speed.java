public class Main {
	final private static long REPS = 10000000;
	final private static int LEN = 1000;

	public static void main(String[] args) {
		int[] z = new int[LEN];
		
		long before = System.nanoTime();
		for(int i = 0; i < REPS; i++) {
			for(int j = 0, x = 0, y = 0; j < LEN; j++, x++, y++) {
				z[j] = x + y * i;
			}
		}
		long after = System.nanoTime();

		System.out.printf("nanos: %.7f\n", (double)(after - before) / (double)REPS);
	}
}
