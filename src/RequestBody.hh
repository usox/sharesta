<?hh // strict
namespace Usox\Sharesta;

use namespace Facebook\TypeAssert;
use namespace HH\Lib\Experimental\IO;

final class RequestBody implements RequestBodyInterface {

  private ?dict<string, mixed> $getBodyCache;
  private ?IO\ReadHandle $input;

  public function useIO(IO\ReadHandle $input): this{
    $this->input = $input;
    $this->getBodyCache = null;
    return $this;
  }

	public function getBody(): dict<string,mixed> {
    if($this->getBodyCache is nonnull){
      return $this->getBodyCache;
    }
    $req_input = IO\request_input();
    $body = $this->input?->rawReadBlocking() ?? $req_input->rawReadBlocking();

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
    /*HH_FIXME[4110] Can I have integer keys?*/
		return dict($value);
	}
}
