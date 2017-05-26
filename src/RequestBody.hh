<?hh // strict
namespace Usox\Sharesta;

final class RequestBody implements RequestBodyInterface {

	<<__Memoize>>
	public function getBody(): Map<string,mixed> {
		$data = Map{};

		$body = file_get_contents('php://input');
		if ($body === '') {
			return $data;
		}
		$json_decoded_body = json_decode($body);

		if (json_last_error() !== JSON_ERROR_NONE) {
			throw new Exception\RequestException('The request body did not contain valid JSON.');
		}
		foreach ($json_decoded_body as $property => $value) {
			$data->add(Pair {$property, $value});
		}
		return $data;
	}
}
