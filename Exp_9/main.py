import os
import sys

# 1. FIX FOR THE UNDERSCORE ERROR (Already done)
os.environ['SPARK_LOCAL_HOSTNAME'] = "localhost"

# 2. FIX FOR THE "PYTHON NOT FOUND" ERROR
# This tells Spark to use the EXACT same Python you are currently using in your venv
os.environ['PYSPARK_PYTHON'] = sys.executable
os.environ['PYSPARK_DRIVER_PYTHON'] = sys.executable

from pyspark.sql import SparkSession

# Start the engine
spark = SparkSession.builder \
    .appName("MyFirstSparkApp") \
    .master("local[*]") \
    .getOrCreate()

print("\n" + "="*30)
print("Spark Session Started Successfully!")
print("="*30 + "\n")

try:
    data = [
        "Hadoop is for storage",
        "Spark is for fast processing",
        "HDFS uses commodity hardware",
        "Spark uses RAM for speed"
    ]

    dist_data = spark.sparkContext.parallelize(data)
    spark_count = dist_data.filter(lambda line: "Spark" in line).count()

    print("-" * 40)
    print(f"RESULT: The word 'Spark' appeared in {spark_count} lines.")
    print("-" * 40)

except Exception as e:
    print(f"Error during processing: {e}")

finally:
    spark.stop()