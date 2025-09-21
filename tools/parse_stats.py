"""
Script to parse multiple softmax statistics text files and collate metrics into a single CSV.

Each input file is expected to contain a list of metric lines, where the first
token in the line identifies the metric and a numeric value appears somewhere
in the line. The script extracts the first numeric value from each line and
associates it with the metric name. All metrics discovered across the input
files are included as columns in the resulting CSV. Two additional columns
are added: `dimension`, which is extracted from the numeric portion of the
filename (e.g. a file named `softmax_N8000.txt` yields dimension `8000`),
and `file`, which stores the base filename. Missing metrics for any
particular file are left blank in the CSV.

Usage:
    python parse_softmax_stats.py file1.txt file2.txt ...

The script writes a file named `softmax_stats.csv` into the current
directory. To change the output filename, modify the script accordingly.
"""

import csv
import os
import re
import sys

# add near the top
import argparse
from pathlib import Path

# ... keep the rest of y

from typing import Dict, List, Optional


def extract_dimension(filename: str) -> Optional[int]:
    """Extract the last integer found in a filename as the problem dimension.

    Examples:
        >>> extract_dimension("softmax_N8000.txt")
        8000
        >>> extract_dimension("stats_65536.txt")
        65536

    If no numeric part is found, returns None.
    """
    numbers = re.findall(r"\d+", filename)
    return int(numbers[-1]) if numbers else None


def parse_metrics_from_file(filepath: str) -> Dict[str, float]:
    """Parse a statistics file and return a mapping of metric names to values.

    The parser is robust to variations in formatting: it assumes the first
    whitespace-separated token of each line is the metric name and scans
    each line for the first numeric value (allowing commas in thousands,
    decimal points and optional exponents). Units or percentage signs after
    the number are ignored. Only the first numeric value is used per line.

    Parameters
    ----------
    filepath : str
        Path to the statistics text file to parse.

    Returns
    -------
    Dict[str, float]
        A dictionary mapping metric names to numeric values.
    """
    metrics: Dict[str, float] = {}
    number_pattern = re.compile(r"[-+]?\d[\d,]*\.?\d*(?:[eE][-+]?\d+)?")
    try:
        with open(filepath, "r", encoding="utf-8", errors="ignore") as f:
            for line in f:
                line = line.strip()
                # Skip empty lines and comment-like lines
                if not line or line.startswith("#"):
                    continue
                # Remove parentheses around metric names and colons
                cleaned = line.replace("(", "").replace(")", "")
                cleaned = cleaned.replace(":", " ")
                tokens = cleaned.split()
                if len(tokens) < 2:
                    continue
                metric_name = tokens[0]
                # Locate the first numeric substring in the cleaned line
                match = number_pattern.search(cleaned)
                if not match:
                    continue
                num_str = match.group(0).replace(",", "")
                try:
                    value = float(num_str)
                except ValueError:
                    # Skip lines that don't contain a parseable number
                    continue
                metrics[metric_name] = value
    except FileNotFoundError:
        print(f"Warning: file not found: {filepath}", file=sys.stderr)
    except Exception as exc:
        print(f"Warning: failed to parse {filepath}: {exc}", file=sys.stderr)
    return metrics


def collate_metrics(filepaths: List[str]) -> List[Dict[str, object]]:
    """Read multiple statistics files and collate metrics into a list of rows.

    For each file the returned row dictionary includes a `dimension` key,
    corresponding to the numeric dimension extracted from the filename, and
    a `file` key with the base filename. All metric names found across
    all files are unified; missing metrics for a given row are absent.

    Parameters
    ----------
    filepaths : List[str]
        A list of paths to statistics files.

    Returns
    -------
    List[Dict[str, object]]
        A list of dictionaries, one for each input file. Each dictionary
        contains metric name/value pairs plus `dimension` and `file` keys.
    """
    rows: List[Dict[str, object]] = []
    for path in filepaths:
        base = os.path.basename(path)
        metrics = parse_metrics_from_file(path)
        metrics["dimension"] = extract_dimension(base)
        metrics["file"] = base
        rows.append(metrics)
    return rows


def write_csv(rows: List[Dict[str, object]], output_path: str = "softmax_stats.csv") -> None:
    """Write the collated metrics to a CSV file.

    Parameters
    ----------
    rows : List[Dict[str, object]]
        List of metric dictionaries returned by `collate_metrics`.
    output_path : str, optional
        The path to the output CSV file. Defaults to 'softmax_stats.csv'.
    """
    # Determine the superset of all metric keys across all rows
    all_keys = set()
    for row in rows:
        all_keys.update(row.keys())
    # Ensure deterministic ordering: dimension and file first, then others sorted
    fieldnames: List[str] = []
    if "dimension" in all_keys:
        fieldnames.append("dimension")
        all_keys.remove("dimension")
    if "file" in all_keys:
        fieldnames.append("file")
        all_keys.remove("file")
    fieldnames.extend(sorted(all_keys))
    # Write the CSV
    with open(output_path, "w", newline="", encoding="utf-8") as csvfile:
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writeheader()
        for row in rows:
            writer.writerow({key: row.get(key, "") for key in fieldnames})
            


# ... keep the rest of your code (extract_dimension, parse_metrics_from_file, collate_metrics, write_csv)

def parse_cli_args(argv):
    p = argparse.ArgumentParser(
        description="Parse multiple gem5 softmax stats files into a single CSV."
    )
    p.add_argument(
        "-o", "--output",
        default="softmax_stats.csv",
        help="Output CSV path (folder + filename). Default: softmax_stats.csv"
    )
    p.add_argument(
        "files",
        nargs="+",
        help="Input stats files to parse (e.g., softmax_N*.txt)"
    )
    return p.parse_args(argv[1:])

def main(argv):
    args = parse_cli_args(argv)
    rows = collate_metrics(args.files)

    out_path = Path(args.output)
    out_path.parent.mkdir(parents=True, exist_ok=True)   # auto-create directories
    write_csv(rows, str(out_path))

if __name__ == "__main__":
    main(sys.argv)


# def main(argv: List[str]) -> None:
#     if len(argv) < 2:
#         print("Usage: python parse_softmax_stats.py <file1> [<file2> ...]", file=sys.stderr)
#         sys.exit(1)
#     filepaths = argv[1:]
#     rows = collate_metrics(filepaths)
#     write_csv(rows)


if __name__ == "__main__":
    main(sys.argv)