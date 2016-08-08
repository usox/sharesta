<?hh // strict
namespace Usox\Sharesta;

final class RequestBody implements RequestBodyInterface {

	<<__Memoize>>
	public function getBody(): Map<string, string> {
		$data = Map{};

		$body = file_get_contents('php://input', 'r');
		$json_decoded_body = json_decode($body);

		if (json_last_error() !== JSON_ERROR_NONE) {
			throw new Exception\RequestException('The request body data was not valid JSON.');
		}
		foreach ($json_decoded_body as $property => $value) {
			$data->add(Pair {$property, $value});
		}
		return $data;
	}
}
