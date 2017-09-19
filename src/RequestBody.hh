<?hh // strict
namespace Usox\Sharesta;

use Facebook\TypeAssert\IncorrectTypeException;
use Facebook\TypeAssert\TypeAssert;

final class RequestBody implements RequestBodyInterface {

	<<__Memoize>>
	public function getBody(): Map<string,mixed> {
		$data = Map{};

		$body = file_get_contents('php://input');
		if ($body === '') {
			return $data;
		}
		$json_decoded_body = json_decode($body, true);

		if (json_last_error() !== JSON_ERROR_NONE) {
			throw new Exception\RequestException('The request body did not contain valid JSON.');
		}
		foreach ($json_decoded_body as $property => $value) {
			$data->add(Pair{$property, $value});
		}
		return $data;
	}

	public function getAsString(string $key): string {
		return TypeAssert::isString($this->getBody()->get($key));
	}

	public function getAsInt(string $key): int {
		return TypeAssert::isInt($this->getBody()->get($key));
	}

	public function getAsBool(string $key): bool {
		return TypeAssert::isBool($this->getBody()->get($key));
	}

	public function getAsVector(string $key): Vector<mixed> {
		$value = $this->getBody()->get($key);
		if (!is_array($value)) {
			throw new IncorrectTypeException('array', gettype($value));
		}
		return new Vector($value);
	}

	public function getAsMap(string $key): Map<mixed, mixed> {
		$value = $this->getBody()->get($key);
		if (!is_array($value)) {
			throw new IncorrectTypeException('array', gettype($value));
		}
		return new Map($value);
	}
}
