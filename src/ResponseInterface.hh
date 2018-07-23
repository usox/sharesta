<?hh // strict
namespace Usox\Sharesta;

interface ResponseInterface {

	public function send(vec<string> $response_header): void;
}
