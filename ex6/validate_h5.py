import sys
import h5py
import numpy as np


if __name__ == "__main__":
    filename = sys.argv[1] if len(sys.argv) > 1 else "chunks.h5"

    with h5py.File(filename, "r") as f:

        print(f"Opening: {filename}")
        print("Datasets found:")
        for name in f.keys():
            print(f"  /{name}")

        # Check expected datasets
        if "chunks" not in f:
            raise RuntimeError("Dataset '/chunks' not found")

        if "partial_sums" not in f:
            raise RuntimeError("Dataset '/partial_sums' not found")

        # Load datasets as NumPy arrays
        chunks = f["chunks"][:]
        partial_sums = f["partial_sums"][:]

        print("\nDataset information:")
        print(f"  chunks:")
        print(f"    shape = {chunks.shape}")
        print(f"    dtype = {chunks.dtype}")

        print(f"  partial_sums:")
        print(f"    shape = {partial_sums.shape}")
        print(f"    dtype = {partial_sums.dtype}")

        # Basic shape consistency
        if chunks.ndim != 2:
            raise RuntimeError(
                f"Expected '/chunks' to be 2D, got {chunks.ndim}D"
            )

        if partial_sums.ndim != 1:
            raise RuntimeError(
                f"Expected '/partial_sums' to be 1D, got {partial_sums.ndim}D"
            )

        num_chunks = chunks.shape[0]

        if len(partial_sums) != num_chunks:
            raise RuntimeError(
                "Number of partial sums does not match number of chunks"
            )

        # Recompute partial sums from the chunks
        calculated_sums = np.sum(chunks, axis=1, dtype=np.float64)

        differences = calculated_sums - partial_sums

        # Compare
        ok = np.allclose(
            calculated_sums,
            partial_sums,
            rtol=1e-7,
            atol=1e-10
        )

        print("\nValidation:")

        if ok:
            print("  [OK] Every chunk sum matches its stored partial sum.")
        else:
            print("  [FAIL] Some chunk sums do not match.")

            bad = np.where(
                ~np.isclose(
                    calculated_sums,
                    partial_sums,
                    rtol=1e-7,
                    atol=1e-10
                )
            )[0]

            for i in bad:
                print(
                    f"    chunk {i}: "
                    f"calculated={calculated_sums[i]:.12g}, "
                    f"stored={partial_sums[i]:.12g}, "
                    f"difference={differences[i]:.12g}"
                )

        # Global sum check
        total_from_chunks = np.sum(chunks, dtype=np.float64)
        total_from_partial_sums = np.sum(partial_sums, dtype=np.float64)

        print("\nGlobal sums:")
        print(f"  from chunks       = {total_from_chunks:.12g}")
        print(f"  from partial sums = {total_from_partial_sums:.12g}")
        print(
            f"  difference        = "
            f"{total_from_chunks - total_from_partial_sums:.12g}"
        )

        # Show a small sample
        print("\nFirst chunk:")
        print(chunks[0])

        print("\nFirst few partial sums:")
        print(partial_sums[:min(5, len(partial_sums))])