import os
import concurrent.futures
import sys

# Puedes cambiar los emojis si no los muestra bien tu terminal o IDE
ok_emoji = ""
error_emoji = ""

time_emoji = "󰥔"
battery_emoji = "󰁹"

# Puedes variar esto de forma tal que la lista tenga al menos dos elementos.
# Si tiene n elementos, los n-1 primeros se distribuyen de forma uniforme,
# siendo los primeros los más bajos. El último elemento corresponde
# a aprobar con un 10 el test.
valoration_emojis = [
    "F",  # 0
    "F",  # 1
    "F",  # 2
    "D",  # 3
    "D",  # 4
    "C",  # 5
    "C",  # 6
    "B",  # 7
    "B",  # 8
    "A",  # 9
    "S",  # 10
]
valoration_len = len(valoration_emojis)


def run_test(test_number, command):
    try:
        output = os.popen(command).read()
        lines = output.strip().split("\n")

        # Obtener los valores necesarios
        map_percentage = float(
            [
                line.split(": ")[-1]
                for line in lines
                if "Porcentaje de mapa descubierto restando errores" in line
            ][0]
        )
        reason = (
            time_emoji
            if "Instantes de simulacion no consumidos: 0" in output
            else battery_emoji
        )
        grade = min(map_percentage / 0.85, 100.0)
        valoration_index = min(
            int(grade * (valoration_len - 1) / 100.0), valoration_len - 1
        )
        valoration = valoration_emojis[valoration_index]

        # Imprimir los resultados
        result_string = f"{test_number:10} {ok_emoji:>10} {map_percentage:10.2f} {reason:>10} {valoration:>10} {grade:10.2f}"
        return result_string
    except Exception:
        error_string = f"{test_number:10} {error_emoji:>10} {0.00:10.2f} {error_emoji:>10} {error_emoji:>10} {-10.00:10.2f}"
        return error_string


def main(tests_file):
    with open(tests_file, "r") as file:
        tests = file.readlines()

    print(f"{'#':>10} {'Éxito':>10} {'%':>10} {'Parada':>10} {'Val':>10} {'Nota':>10}")
    print("=" * 70)

    with concurrent.futures.ThreadPoolExecutor() as executor:
        futures = []
        results = []
        for test_number, test in enumerate(tests, start=1):
            args = test.strip().split()
            command = f"./practica1SG {' '.join(args)}"
            futures.append(executor.submit(run_test, test_number, command))

        for future in concurrent.futures.as_completed(futures):
            result = future.result()
            if result:
                results.append(result)

    results.sort()
    for result in results:
        print(result)


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Uso: python3 test_script.py <tests_file>")
        sys.exit(1)
    tests_file = sys.argv[1]
    main(tests_file)
