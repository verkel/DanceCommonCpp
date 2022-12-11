export module MathUtils;

export namespace DanceCommon
{
	export class MathUtils
	{
	public:

		/**
		 * Calculate greatest common divisor
		 */
		static int GCD(int m, int n) {
			return m % n == 0 ? n : GCD(n, m % n);
		}

		/**
		 * Calculate least common multiple
		 */
		static int LCM(int m, int n) {
			return m * (n / GCD(m, n));
		}

		/**
		 * Returns the number of one-bits in the two's complement binary
		 * representation of the specified {@code int} value.  This function is
		 * sometimes referred to as the <i>population count</i>.
		 *
		 * @return the number of one-bits in the two's complement binary
		 *     representation of the specified {@code int} value.
		 * @since 1.5
		 */
		static int BitCount(int i) {
			// (From java.lang.Integer)
			// HD, Figure 5-2
			i = i - ((i >> 1) & 0x55555555);
			i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
			i = (i + (i >> 4)) & 0x0f0f0f0f;
			i = i + (i >> 8);
			i = i + (i >> 16);
			return i & 0x3f;
		}
	};
}