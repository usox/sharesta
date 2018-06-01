<?hh // strict
namespace Usox\Sharesta;

interface RequestInterface {

	public function getRoute(string $base_path): string;

	public function getHttpMethod(): string;

	public function getRouteParameters(): dict<string, string>;

	public function setRouteParameters(dict<string, string> $route_parameters): void;

	public function getUriValues(): dict<string, string>;

	public function getBodyAsJson(): string;

	public function getBody(): dict<string, mixed>;
}
