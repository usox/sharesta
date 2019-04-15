<?hh // strict
namespace Usox\Sharesta;

use namespace Facebook\TypeAssert;

final class RequestBody implements RequestBodyInterface {

	<<__Memoize>>
	public function getBody(): dict<string,mixed> {
		$body = \file_get_contents('php://input');
		if ($body === '') {
			return dict([]);
		}
		$json_decoded_body = \json_decode($body, true);

		if (\json_last_error() !== \JSON_ERROR_NONE) {
			throw new Exception\RequestException('The request body did not contain valid JSON.');
		}

		return dict($json_decoded_body);
	}

	public function getAsString(string $key): string {
		try {
			return TypeAssert\string($this->getBody()[$key]);
		} catch (\Exception $e) {
			throw new Exception\Request\InvalidRequestParamException('Invalid parameter for key '.$key);
		}
	}

	public function getAsInt(string $key): int {
		try {
			return TypeAssert\int($this->getBody()[$key]);
		} catch (\Exception $e) {
			throw new Exception\Request\InvalidRequestParamException('Invalid parameter for key '.$key);
		}
	}

	public function getAsBool(string $key): bool {
		try {
			return TypeAssert\bool($this->getBody()[$key]);
		} catch (\Exception $e) {
			throw new Exception\Request\InvalidRequestParamException('Invalid parameter for key '.$key);
		}
	}

	public function getAsVec(string $key): vec<mixed> {
		$value = $this->getBody()[$key];
		if (!\is_array($value)) {
			throw new Exception\Request\InvalidRequestParamException('Invalid parameter for key '.$key);
		}
		return vec($value);
	}

	public function getAsDict(string $key): dict<string, mixed> {
		$value = $this->getBody()[$key];
		if (!\is_array($value)) {
			throw new Exception\Request\InvalidRequestParamException('Invalid parameter for key '.$key);
		}
		return dict($value);
	}
}
