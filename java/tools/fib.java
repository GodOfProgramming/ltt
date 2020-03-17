public class Main {
	public static void main(String[] args) {
		if (args.length < 1) {
			System.out.println("You need to supply an argument");
			System.exit(1);
		}

		int count = Integer.parseInt(args[1]);

		if (count < 0) {
			printf("You need to use a positive number\n");
			System.exit(1);
		}

		int result = fib(count);

		System.out.printf("%d", result);
	}

	private static int fib(int count) {
		int prev = 0;
		int curr = 1;

		for(int i = 0; i < count; i++) {
			int tmp = curr;
			curr += prev;
			prev = tmp;
		}

		return prev;
	}
}
