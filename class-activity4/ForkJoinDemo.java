import java.util.concurrent.*;

public class ForkJoinDemo {

    static class SumTask extends RecursiveTask<Long> {
        long start, end;
        static final long THRESHOLD = 200_000;

        SumTask(long start, long end) {
            this.start = start;
            this.end = end;
        }

        @Override
        protected Long compute() {
            long len = end - start + 1;
            if (len <= THRESHOLD) {
                long sum = 0;
                for (long i = start; i <= end; i++) sum += i;
                System.out.println("Computed [" + start + "," + end + "] by " + Thread.currentThread().getName());
                return sum;
            }
            long mid = (start + end) / 2;
            SumTask left = new SumTask(start, mid);
            SumTask right = new SumTask(mid + 1, end);

            left.fork();
            long r = right.compute();
            long l = left.join();
            return l + r;
        }
    }

    public static void main(String[] args) {
        ForkJoinPool pool = new ForkJoinPool();
        System.out.println("Parallelism = " + pool.getParallelism());

        long n = 2_000_000L;
        long result = pool.invoke(new SumTask(1, n));
        System.out.println("Sum 1.." + n + " = " + result);

        pool.shutdown();
    }
}
