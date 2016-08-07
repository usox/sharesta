<?hh // strict
namespace Usox\Sharesta;

final class RequestBody implements RequestBodyInterface {

	<<__Memoize>>
	public function getBody(): Map<string, string> {
		$data = Map{};

		$body = file_get_contents('php://input', 'r');

		if ($json_decoded_body = json_decode($body)) {
			foreach ($json_decoded_body as $property => $value) {
				$data->add(Pair {$property, $value});
			}
		} else if ($body) {
			throw new Exception\RequestException('The request body data was not valid JSON.');
		}
		return $data;
	}
}
